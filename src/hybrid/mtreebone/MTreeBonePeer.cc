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

            if (deny->getRequestThis() > 0){
                requestChunk(src, deny->getRequestThis());
            }
            //rerequestChunk(deny->getSequenceNumber(), src);
            break;
        default:
        {
            MTreeBoneBase::processPacket(pkt);
            break;
        }
    }

    if (csp->getPacketType() == MTREEBONE_BUFFER_MAP){
        requestNextChunk(src);
        //requestChunks(src);
    }
}

void MTreeBonePeer::requestChunks(IPvXAddress peer){

    MTreeBonePeerInformation* info = getPeerInformation(peer);
    if (info == NULL) return;

    int requests = m_ChunksPerSecond * 1.5;
    for (int i = info->getSequenceNumberEnd(); i >= info->getSequenceNumberStart(); i--){
        if (info->inBuffer(i))
            if (requestChunk(peer, i))
                requests--;
        if (requests <= 0)
            break;
    }

    EV << endl << "MTreeBonePeer::requestChunks @ " << m_localAddress.str() << " -> " << requests << endl << endl;
}

void MTreeBonePeer::onNewChunk(IPvXAddress src, int sequenceNumber){
    MTreeBoneBase::onNewChunk(src, sequenceNumber);

    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);
    MTreeBonePeerInformation* info = getPeerInformation(src);
    EV << endl << " MTreeBonePeer::onNewChunk @ " << m_localAddress.str() << " -> " << sequenceNumber << " , " << (info == NULL) << endl << endl;
    if (info != NULL){
        info->nextRequestTime = 0;
        requestNextChunk(src, 1);
    }
    simulation.setContext(origContext);
}

void MTreeBonePeer::requestNextChunk(IPvXAddress peer, int max){
    MTreeBonePeerInformation* info = getPeerInformation(peer);
    if (info == NULL) return;

    // last Chunk pending
    if (info->nextRequestTime > simTime())
        return;

    int count = 0;

    for (int i = info->getSequenceNumberStart(); i < info->getSequenceNumberEnd(); i++){
        if (info->inBuffer(i))
            if (requestChunk(peer, i)){
                count++;
                info->nextRequestTime = simTime() + 0.4;
                if (count >= max)
                    return;
            }
    }

    if (count == 0)
        EV << endl << " MTreeBonePeer::requestNextChunk @ " << m_localAddress.str() << " NO REQUEST POSSIBLE FROM " << peer.str() << endl << endl << endl ;
}
bool MTreeBonePeer::requestChunk(IPvXAddress peer, int sequenceNumber){

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

    EV << endl << " MTreeBonePeer::requestChunk @ " << m_localAddress.str() << " -> " << sequenceNumber << endl << endl;

    MTreeBoneChunkRequestPacket* req = new MTreeBoneChunkRequestPacket();
    req->setSequenceNumber(sequenceNumber);
    sendToDispatcher(req, m_localPort, peer, m_destPort);

    return true;
}

void MTreeBonePeer::rerequestChunk(int sequenceNumber, IPvXAddress notHim){

    int stripe = sequenceNumber % param_numStripes;

    if (m_Stripes[stripe].Neighbors.size() <= 1) // only one or none neighbor? -> exit
        return;

    for (unsigned int i = 0; i < m_Stripes[stripe].Neighbors.size(); i++){
        int rnd = (int)intrand(m_Stripes[stripe].Neighbors.size());

        if (m_Stripes[stripe].Neighbors.at(rnd).equals(notHim))
            continue;

        if (requestChunk(m_Stripes[stripe].Neighbors.at(rnd), sequenceNumber)){
            EV << endl << endl << endl << "     MTreeBonePeer::rerequestChunk " << sequenceNumber << " from " <<m_Stripes[stripe].Neighbors.at(rnd).str() << " not " << notHim.str() << endl << endl << endl << endl;
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
