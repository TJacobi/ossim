//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "MTreeBonePeer.h"

inline int max(const int a, const int b) { return (a > b) ? a : b; }

Define_Module(MTreeBonePeer);

MTreeBonePeer::MTreeBonePeer(){

}

MTreeBonePeer::~MTreeBonePeer() {
    if (timer_joinNetwork)
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = 0;
}

void MTreeBonePeer::initialize(int stage){
    if (stage != 3)
        return;
    initBase();

    param_DisablePush = par("disablePush");
    // create messages
    timer_joinNetwork    = new cMessage("MTreeBonePeer_JOIN_NETWORK");
    timer_checkNeighbors = new cMessage("MTreeBonePeer_CHECK_NEIGHBORS");

    // schedule timers
    scheduleAt(simTime() + uniform(0,5), timer_joinNetwork);
}

void MTreeBonePeer::handleTimerMessage(cMessage *msg){

    if (msg == timer_joinNetwork)
    {
        cancelEvent(timer_joinNetwork);

        scheduleAt(simTime() + uniform(0,5), timer_checkNeighbors);
    }
    else if (msg == timer_checkNeighbors){
        checkNeighbors();
        checkParents();
        scheduleAt(simTime() + 5, timer_checkNeighbors);
    }
    else
    {
        MTreeBoneBase::handleTimerMessage(msg);
    }
}

void MTreeBonePeer::processPacket(cPacket *pkt){
    MTreeBonePacket* csp = dynamic_cast<MTreeBonePacket*>(pkt);
    if (csp == NULL) return;

    MTreeBoneChunkDenyPacket* deny ;
    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    switch (csp->getPacketType()){
        case MTREEBONE_CHUNK_DENY: // handle deny
            deny = check_and_cast<MTreeBoneChunkDenyPacket*> (pkt);
            m_PendingRequests.erase(deny->getSequenceNumber());

            m_outFileDebug << simTime().str() << " [DOWN] our chunk request denied " << deny->getSequenceNumber() << " from " << src.str() << endl;
            if (deny->getRequestThis() > 0){
                m_outFileDebug << simTime().str() << " [DOWN] reroutet to " << deny->getRequestThis() << " from " << src.str() << endl;
                if (deny->getRequestThis() < (unsigned int)m_videoBuffer->getBufferStartSeqNum())
                    m_outFileDebug << simTime().str() << " [DOWN] sadly its before our buffer start ..." << endl;
                else
                    requestChunkFromPeer(src, deny->getRequestThis());
            }
            break;
        case MTREEBONE_PARENT_REQUEST_RESPONSE: // handle response
            handleParentRequestResponse(src, check_and_cast<MTreeBoneParentRequestResponsePacket *> (pkt) );
            break;
        default:
        {
            MTreeBoneBase::processPacket(pkt);
            break;
        }
    }

    if (csp->getPacketType() == MTREEBONE_BUFFER_MAP){
        if (m_videoBuffer->getBufferStartSeqNum() == 0){ // if we are behind set starting point to head - 2 seconds
            m_videoBuffer->setBufferStartSeqNum( max((check_and_cast<MTreeBoneBufferMapPacket *> (pkt))->getSequenceNumberEnd()- m_ChunksPerSecond*2,0));
        }
        requestNextChunks(src);
    }
}

void MTreeBonePeer::onNewChunk(IPvXAddress src, int sequenceNumber){
    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);
    MTreeBonePeerInformation* info = getPeerInformation(src);
    EV << endl << " MTreeBonePeer::onNewChunk @ " << m_localAddress.str() << " -> " << sequenceNumber << " , " << (info == NULL) << endl << endl;

    m_outFileDebug << simTime().str() << " [DOWN] got new chunk " << sequenceNumber << " from " << src.str() << endl;

    if (info != NULL){
        info->nextRequestTime = 0;
        requestNextChunks(src, 1);
    }
    simulation.setContext(origContext);

    MTreeBoneBase::onNewChunk(src, sequenceNumber);
}

void MTreeBonePeer::requestNextChunks(IPvXAddress peer, int maxRequests){
    MTreeBonePeerInformation* info = getPeerInformation(peer);
    if (info == NULL) return;

    // last Chunk pending
    if (info->nextRequestTime > simTime())
        return;

    int count = 0;

    for (int i = max(info->getSequenceNumberStart(), m_videoBuffer->getBufferStartSeqNum()); i < info->getSequenceNumberEnd(); i++){
        if (info->inBuffer(i))
            if (requestChunkFromPeer(peer, i)){
                count++;
                info->nextRequestTime = simTime() + 0.4;
                if (count >= maxRequests)
                    return;
            }
    }

    if (count == 0)
        EV << endl << " MTreeBonePeer::requestNextChunk @ " << m_localAddress.str() << " NO REQUEST POSSIBLE FROM " << peer.str() << endl << endl << endl ;
}
bool MTreeBonePeer::requestChunkFromPeer(IPvXAddress peer, int sequenceNumber){

    // check if we already have that chunk
    if (m_videoBuffer->inBuffer(sequenceNumber))
        return false;

    // check if we already requested that chunk
    std::map<int, SimTime>::iterator it = m_PendingRequests.find(sequenceNumber);
    if ((it != m_PendingRequests.end()) && ( it->second > simTime() )) // request is already pending
        return false;

    // check if he is a neighbor of that stripe
    int stripe = sequenceNumber % param_numStripes;
    if (!m_Stripes[stripe].Neighbors.containsItem(peer))
        return false;

    m_PendingRequests.erase(sequenceNumber);
    m_PendingRequests.insert(std::pair<int, SimTime>(sequenceNumber, simTime()+1));

    EV << endl << " MTreeBonePeer::requestChunkFromPeer @ " << m_localAddress.str() << " -> " << sequenceNumber << endl << endl;

    m_outFileDebug << simTime().str() << " [DOWN] requesting chunk " << sequenceNumber << " from " << peer.str() << endl;

    MTreeBoneChunkRequestPacket* req = new MTreeBoneChunkRequestPacket();
    req->setSequenceNumber(sequenceNumber);
    sendToDispatcher(req, m_localPort, peer, m_destPort);

    return true;
}

void MTreeBonePeer::requestChunk(int sequenceNumber, IPvXAddress notHim){

    int stripe = sequenceNumber % param_numStripes;

    if (m_Stripes[stripe].Neighbors.size() <= 1) // only one or none neighbor? -> exit
        return;

    IPvXAddress addr;
    MTreeBonePeerInformation* info;


    for (unsigned int i = 0; i < m_Stripes[stripe].Neighbors.size(); i++){
        int rnd = (int)intrand(m_Stripes[stripe].Neighbors.size());
        addr = m_Stripes[stripe].Neighbors.at(rnd);

        if (addr.equals(notHim))
            continue;

        info = getPeerInformation(addr);

        // last Chunk pending
        if (info->nextRequestTime > simTime())
            continue;

        if (info->inBuffer(sequenceNumber) && (requestChunkFromPeer(addr, sequenceNumber))){
            EV << endl << endl << endl << "     MTreeBonePeer::requestChunk " << sequenceNumber << " from " << addr.str() << " not " << notHim.str() << endl << endl << endl << endl;
            break;
        }
    }
}

void MTreeBonePeer::checkNeighbors(){
    EV << "TreeBonePeer::checkNeighbors() @ " << m_localAddress.str() << endl;
    MTreeBonePeerInformation* info;
    std::vector<IPvXAddress>::iterator it;

    for (unsigned int i = 0; i < param_numStripes; i++){
        EV << "  Stripe: " << i << " partners: " << m_Stripes[i].Neighbors.size() << endl;
        if (m_Stripes[i].Neighbors.size() >= param_desiredNOP){ // has enough neighbors, check if we want to kick someone
            for (it = m_Stripes[i].Neighbors.begin(); it != m_Stripes[i].Neighbors.end(); it++){
                info = getPeerInformation( *it );
                if ((info == NULL) || (info->isProbablyDesertedPeer())){
                    MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
                    resp->setStripeNumber(i);
                    resp->setIsAccepted(false);
                    sendToDispatcher(resp, m_localPort, *it, m_destPort);

                    removeNeighbor( *it, i);
                    break;
                }
            }
        }

        if (m_Stripes[i].Neighbors.size() < param_desiredNOP){ // stripe needs/wants more neighbors
            IPvXAddress addr = m_Gossiper->getRandomPeer(m_localAddress);
            if ((!addr.isUnspecified()) && (!m_Stripes[i].Neighbors.containsItem(addr))){
                MTreeBoneNeighborRequestPacket* request = new MTreeBoneNeighborRequestPacket();
                request->setStripeNumber(i);
                sendToDispatcher(request, m_localPort, addr, m_destPort);
            }
        }else if(m_Stripes[i].Neighbors.size() > param_maxNOP){ // too many neighbors? shouldnt be possible ..

        }
    }
}

void MTreeBonePeer::checkParents(){

    IPvXAddress addr;
    MTreeBonePeerInformation* info;

    for (unsigned int stripe = 0; stripe < param_numStripes; stripe ++){
        if (m_Stripes[stripe].nextParentRequest > simTime()) // not waited long enough ...
            continue;

        if (!wantToBeBoneNode(stripe)) // we dont want to be a bone node?
            continue;

        if (m_Stripes[stripe].isBoneNode()) // we have a parent
            continue;

        for (unsigned int i = 0; i < m_Stripes[stripe].Neighbors.size(); i++){
            int rnd = (int)intrand(m_Stripes[stripe].Neighbors.size());
            addr = m_Stripes[stripe].Neighbors.at(rnd);

            info = getPeerInformation(addr);
            if ( (info == NULL) || (!info->isBoneNode(stripe)) ) // no peer information or peer is not a bone node?
                continue;

            m_Stripes[stripe].nextParentRequest = simTime() + 3;
            MTreeBoneParentRequestPacket* req = new MTreeBoneParentRequestPacket();
            req->setStripeNumber(stripe);
            sendToDispatcher(req, m_localPort, addr, m_destPort);
        }
    }
}

void MTreeBonePeer::handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp){

    if (resp->getIsAccepted())
        m_Stripes[resp->getStripeNumber()].Parent = src;

}
