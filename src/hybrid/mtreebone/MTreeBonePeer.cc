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
    globalUp = globalDown = 0;
    m_PlayerPosition = 0;
}

MTreeBonePeer::~MTreeBonePeer() {
    if (timer_joinNetwork)
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;
    if (timer_checkNeighbors)
            cancelAndDelete(timer_checkNeighbors); timer_checkNeighbors = NULL;
    if (timer_chunkScheduler)
            cancelAndDelete(timer_chunkScheduler); timer_chunkScheduler = NULL;
}

void MTreeBonePeer::initialize(int stage){
    if (stage != 3)
        return;
    initBase();

    param_DisablePush = par("disablePush");
    param_ChunkScheduleInterval = par("ScheduleInterval");
    // create messages
    timer_joinNetwork    = new cMessage("MTreeBonePeer_JOIN_NETWORK");
    timer_checkNeighbors = new cMessage("MTreeBonePeer_CHECK_NEIGHBORS");
    timer_chunkScheduler = new cMessage("MTreeBonePeer_CHUNK_SCHEDULER");

    // schedule timers
    scheduleAt(simTime() + uniform(0,5), timer_joinNetwork);
}

void MTreeBonePeer::handleTimerMessage(cMessage *msg){

    if (msg == timer_joinNetwork)
    {
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;

        scheduleAt(simTime() + uniform(0,5), timer_checkNeighbors);
        scheduleAt(simTime() + 1, timer_chunkScheduler);
    }
    else if (msg == timer_checkNeighbors){
        checkNeighbors();
        checkParents();
        scheduleAt(simTime() + 1, timer_checkNeighbors);
    }
    else if (msg == timer_chunkScheduler){
        doChunkSchedule();
        scheduleAt(simTime() + m_videoBuffer->getChunkInterval() * param_ChunkScheduleInterval, timer_chunkScheduler);
    }
    else
    {
        MTreeBoneBase::handleTimerMessage(msg);
    }
}

void MTreeBonePeer::processPacket(cPacket *pkt){
    MTreeBonePacket* csp = dynamic_cast<MTreeBonePacket*>(pkt);
    if (csp == NULL) return;

    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    switch (csp->getPacketType()){
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
        if (m_videoBuffer->getBufferStartSeqNum() == 0){ // if we are behind set starting point to head - x second(s)
            // TODO: make it better ... maybe head - 50% buffersize?
            //m_videoBuffer->setBufferStartSeqNum( max((check_and_cast<MTreeBoneBufferMapPacket *> (pkt))->getSequenceNumberEnd() - m_ChunksPerSecond*1 , 0));
            //m_PlayerPosition = max((check_and_cast<MTreeBoneBufferMapPacket *> (pkt))->getSequenceNumberEnd() - m_ChunksPerSecond*3 , 0);
            //m_PlayerPosition = max((check_and_cast<MTreeBoneBufferMapPacket *> (pkt))->getSequenceNumberEnd() - m_videoBuffer->getSize()/2 , 0);
        }
    }
}

void MTreeBonePeer::onNewChunk(IPvXAddress src, int sequenceNumber){
    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);
    MTreeBonePeerInformation* info = getPeerInformation(src);
    EV << endl << " MTreeBonePeer::onNewChunk @ " << m_localAddress.str() << " -> " << sequenceNumber << " , " << (info == NULL) << endl << endl;

    m_outFileDebug << simTime().str() << " [DOWN] got new chunk " << sequenceNumber << " from " << src.str() << endl;

    if (info != NULL){
        info->chunksReceived++;
    }
    simulation.setContext(origContext);

    globalDown++;

    MTreeBoneBase::onNewChunk(src, sequenceNumber);
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

                    m_outFileDebug << simTime().str() << " [NEIGHBOR] remove deserted peer: " << (*it).str() << " for stripe: " << i << endl;

                    removeNeighbor( *it, i);
                    break;
                }
            }
        }

        if (m_Stripes[i].Neighbors.size() < param_desiredNOP){ // stripe needs/wants more neighbors
            for (int j = 0; j < (param_desiredNOP - m_Stripes[i].Neighbors.size()); j++){
                IPvXAddress addr = m_Gossiper->getRandomPeer(m_localAddress);
                if ((!addr.isUnspecified()) && (!m_Stripes[i].Neighbors.containsItem(addr))){
                    MTreeBoneNeighborRequestPacket* request = new MTreeBoneNeighborRequestPacket();
                    request->setStripeNumber(i);
                    sendToDispatcher(request, m_localPort, addr, m_destPort);
                }
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

            m_outFileDebug << simTime().str() << " [PARENT] send request to " << addr.str() << " for stripe: "<< req->getStripeNumber() << endl;

            sendToDispatcher(req, m_localPort, addr, m_destPort);
            break;
        }
    }
}

void MTreeBonePeer::handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp){

    m_outFileDebug << simTime().str() << " [PARENT] got response from " << src.str() << " for stripe: " << resp->getStripeNumber() << " status: "<< (resp->getIsAccepted() ? "accepted" : "denied") << endl;
    if (resp->getIsAccepted())
        m_Stripes[resp->getStripeNumber()].Parent = src;

}

void MTreeBonePeer::doChunkSchedule(){
    int head = getHeadSequenceNumber();
    // adjust position ... TODO: check if this ok and how to alter player
    if (
            (m_PlayerPosition < head - m_videoBuffer->getSize()*0.75)||
            ((m_PlayerPosition > head - m_videoBuffer->getSize()*0.25) && (head > m_videoBuffer->getSize() * 0.5))
    ){
        m_outFileDebug << simTime().str() << " [PLAYING] adjust player position from " << m_PlayerPosition << " to " << (head - m_videoBuffer->getSize()/2) << endl;
        m_PlayerPosition = head - m_videoBuffer->getSize()/2;
    }

    for (unsigned int i = 0; i < param_numStripes; i++)
        doChunkSchedule(i);

    if (head > m_ChunksPerSecond * 2) // wait till head is atleast 2 seconds ahead
        m_PlayerPosition += param_ChunkScheduleInterval;
}

void inline addRequestToMap(std::map<IPvXAddress, genericList<int> >* list, IPvXAddress addr, unsigned int chunk){
    std::map<IPvXAddress, genericList<int> >::iterator it = list->find(addr);

    if (it == list->end()){ // no entry
        genericList<int> insert;
        insert.addItem(chunk);
        list->insert(std::pair<IPvXAddress, genericList<int> >(addr, insert));
    }else{
        it->second.addItem(chunk);
    }
}

int inline getRequestCountInMap(std::map<IPvXAddress, genericList<int> >* list, IPvXAddress addr){
    std::map<IPvXAddress, genericList<int> >::iterator it = list->find(addr);

    if (it == list->end()){ // no entry
        return 0;
    }else{
        return it->second.size();
    }
}


void MTreeBonePeer::doChunkSchedule(unsigned int stripe){
    unsigned int oldestChunk = ((m_PlayerPosition % param_numStripes) == stripe)? m_PlayerPosition : m_PlayerPosition + (stripe - m_PlayerPosition % param_numStripes) + param_numStripes;
    unsigned int newestChunk = m_PlayerPosition + m_videoBuffer->getSize()/2;

    if (m_Stripes[stripe].isBoneNode())
        newestChunk = max(m_videoBuffer->getHeadReceivedSeqNum() - m_ChunksPerSecond, oldestChunk + m_ChunksPerSecond * 3);
    m_outFileDebug << simTime().str() << " [doChunkSchedule] from " << oldestChunk << " to " << newestChunk << " for stripe: " << stripe << endl;

    IPvXAddress addr;
    MTreeBonePeerInformation* info;
    std::map<IPvXAddress, genericList<int> >* requests = new std::map<IPvXAddress, genericList<int> >();
    int numberOfRequests = 0;

    // loop through the first chunk we require until the last chunk we need at the moment
    // TODO: look for push AND critical regions!
    // 1. loop = look for chunks with only one neighbor owning them
    for (unsigned int chunk = oldestChunk; chunk < newestChunk; chunk++){
        if ( (!m_videoBuffer->inBuffer(chunk)) && (! requestIsPending(chunk) ) ) // we dont have that chunk + request is not pending
            if (getNumberOfPeersWithChunk(stripe, chunk) == 1){ // it only exists one peer who has that chunk
                addr = getRandomPeerWithChunk(stripe, chunk); // get that peer
                m_outFileDebug << simTime().str() << " [DOWN] request chunk from single source " << chunk << " , " << addr.str() << endl;
                if (getRequestCountInMap(requests, addr) < 25){
                    addRequestToMap(requests, addr, chunk);       // add him to the request list
                    numberOfRequests++;
                }
            }
    }

    // 2. loop = look for chunks with only one neighbor owning them
    for (unsigned int chunk = oldestChunk; chunk < newestChunk; chunk++){
        if ( (!m_videoBuffer->inBuffer(chunk)) && (! requestIsPending(chunk) ) ) // we dont have that chunk + request is not pending
            if (getNumberOfPeersWithChunk(stripe, chunk) > 1){ // only if multiply copies exist ...
//                m_outFileDebug << simTime().str() << " [DOWNDEBUG] chunk " << chunk << " peers " <<  getNumberOfPeersWithChunk(stripe, chunk) << endl;

                std::vector<IPvXAddress>::iterator it;
                IPvXAddress best; double bestQuality = -1;
                for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
                    info = getPeerInformation(*it);
//                    m_outFileDebug << simTime().str() << " [DOWNDEBUG] address " << (*it).str() << " requests " << getRequestCountInMap(requests, *it) << endl;
                    if ( (info->inBuffer(chunk)) && (getRequestCountInMap(requests, *it) < 15)){
                        double quality = ((numberOfRequests+1) - getRequestCountInMap(requests, *it));
                                quality *= info->getQuality();
                                quality /= (double)numberOfRequests;
                        if ( (*it).equals(m_Stripes[stripe].Parent)) quality *= 0.25;

//                        m_outFileDebug << simTime().str() << " [DOWNDEBUG] number req " << numberOfRequests << " peer quality: " << info->getQuality() << endl;
//                        m_outFileDebug << simTime().str() << " [DOWNDEBUG] quality " << quality << endl;

                        if (quality > bestQuality){
                            bestQuality = quality;
                            best = *it;
                        }
                    }
                }

                if (bestQuality > 0){
                    m_outFileDebug << simTime().str() << " [DOWN] picking " << best.str() << " for chunk " << chunk << " quality: " << bestQuality << endl;
                    addRequestToMap(requests, best, chunk);
                    numberOfRequests++;
                }
            }
    }

    // now prepare and send requests
    std::map<IPvXAddress, genericList<int> >::iterator iter;
    for (iter = requests->begin(); iter != requests->end(); iter++){
        if (iter->second.size() == 0) continue;

        MTreeBoneChunkRequestListPacket* pkt = new MTreeBoneChunkRequestListPacket();
        pkt->setSequenceNumbersArraySize(iter->second.size());
        for (unsigned int i = 0; i < iter->second.size(); i++){
            m_PendingRequests.insert(std::pair<int, SimTime>(iter->second.at(i), simTime()+0.5));
            pkt->setSequenceNumbers(i, iter->second.at(i));
        }

        info = getPeerInformation(iter->first);
        info->requestsSend += iter->second.size();

        m_outFileDebug << simTime().str() << " [DOWN] sending request list to " << iter->first.str() << " count: " << iter->second.size() << endl;

        sendToDispatcher(pkt, m_localPort, iter->first, m_destPort);
    }

    delete requests;
}


int MTreeBonePeer::getNumberOfPeersWithChunk(int stripe, int chunk){
    std::vector<IPvXAddress>::iterator it;
    MTreeBonePeerInformation* info;

    int ret = 0;

    for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
        info = getPeerInformation(*it);
        if (info->inBuffer(chunk))
            ret++;
    }

    return ret;
}

IPvXAddress MTreeBonePeer::getRandomPeerWithChunk(int stripe, int chunk){
    std::vector<IPvXAddress>::iterator it;
    MTreeBonePeerInformation* info;

    for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
        info = getPeerInformation(*it);
        if (info->inBuffer(chunk))
            return (*it);
    }

    return IPvXAddress("0.0.0.0");
}

bool MTreeBonePeer::requestIsPending(unsigned int sequenceNumber){
    //m_outFileDebug << simTime().str() << " [requestIsPending] a " << sequenceNumber << endl;
    // check if we already requested that chunk
    std::map<int, SimTime>::iterator it = m_PendingRequests.find(sequenceNumber);
    if (it == m_PendingRequests.end())
        return false;

    //m_outFileDebug << simTime().str() << " [requestIsPending] b " << sequenceNumber << " timeout: " << it->second << endl;

    if (it->second < simTime() ){ // timed out
        m_outFileDebug << simTime().str() << " [DOWN] timed out: " << sequenceNumber << endl;
        m_PendingRequests.erase(it);
        return false;
    }

    //m_outFileDebug << simTime().str() << " [requestIsPending] c " << sequenceNumber << endl;

    return true;
}
