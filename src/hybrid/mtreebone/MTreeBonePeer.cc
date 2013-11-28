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

#include "MTreeBoneStats.h"

MTreeBonePeer::MTreeBonePeer(){
    globalUp = globalDown = 0;
    m_PlayerPosition = 0;
}

MTreeBonePeer::~MTreeBonePeer() {
    if (timer_joinNetwork)
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;
    if (timer_leaveNetwork)
        cancelAndDelete(timer_leaveNetwork); timer_leaveNetwork = NULL;
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
    param_ChunkScheduleInterval = par("ChunkScheduleInterval");
    param_ChunkRequestTimeout   = par("ChunkRequestTimeout");

    m_ChunksReceived = 0;

    //MTreeBoneStats::theStats->addPeer(this);

    //get player pointer:
    //mPlayer = check_and_cast<PlayerStallSkip*>(getParentModule()->getModuleByRelativePath("player"));
    mPlayer = check_and_cast<PlayerBufferSkip*>(getParentModule()->getModuleByRelativePath("player"));

    mPlayer->addListener(this);
    // create messages
    std::string temp = "MTreeBonePeer_CHECK_NEIGHBORS" + m_localAddress.str();
    timer_joinNetwork    = new cMessage("MTreeBonePeer_JOIN_NETWORK");
    timer_leaveNetwork   = new cMessage("MTreeBonePeer_LEAVE_NETWORK");
    timer_checkNeighbors = new cMessage(temp.c_str());
    timer_chunkScheduler = new cMessage("MTreeBonePeer_CHUNK_SCHEDULER");

    // schedule timers
    //scheduleAt(simTime() + uniform(0,5), timer_joinNetwork);
    scheduleAt(par("joinTime").doubleValue(), timer_joinNetwork);
    if (par("stayDuration").doubleValue() > 0)
        scheduleAt(par("joinTime").doubleValue() + par("stayDuration").doubleValue(), timer_leaveNetwork);

}

void MTreeBonePeer::handleTimerMessage(cMessage *msg){

    if (msg == timer_joinNetwork)
    {
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;
        m_Gossiper->joinNetwork();

        MTreeBoneStats::theStats->peerJoinedNetwork(this);
        scheduleAt(simTime() + uniform(0,5), timer_checkNeighbors);
        scheduleAt(simTime() + 1, timer_chunkScheduler);
    }
    else if (msg == timer_leaveNetwork){
        cancelAndDelete(timer_leaveNetwork); timer_leaveNetwork = NULL;

        if (timer_checkNeighbors) cancelAndDelete(timer_checkNeighbors); timer_checkNeighbors = NULL;
        if (timer_chunkScheduler) cancelAndDelete(timer_chunkScheduler); timer_chunkScheduler = NULL;
        //MTreeBoneStats::theStats->removePeer(this); // unregister
        MTreeBoneStats::theStats->peerLeavedNetwork(this);
        m_Gossiper->leaveNetwork();
        for (unsigned int stripe = 0; stripe < param_numStripes; stripe++){
            removeParent(stripe); // inform stripe parent that we are no longer a child
            while(m_Stripes[stripe].Children.size() > 0)
                removeChild(stripe, m_Stripes[stripe].Children.at(0));
            //for (unsigned int child = 0; child < m_Stripes[stripe].Children.size(); child++)
                //removeChild(stripe, m_Stripes[stripe].Children.at(child));
            for (unsigned int neighbor = 0; neighbor < m_Stripes[stripe].Neighbors.size(); neighbor++){
                MTreeBoneNeighborRequestResponsePacket* drop = new MTreeBoneNeighborRequestResponsePacket();
                drop->setStripeNumber(stripe);
                drop->setIsAccepted(false);
                sendToDispatcher(drop, m_localPort, m_Stripes[stripe].Neighbors.at(neighbor), m_destPort);
            }
        }

        m_PendingRequests.clear();
        mPlayer->stopPlayer();
    }
    else if (msg == timer_checkNeighbors){
        checkNeighbors();
        checkParents();
        scheduleAt(simTime() + 1, timer_checkNeighbors);
    }
    else if (msg == timer_chunkScheduler){
        doChunkSchedule();
        //if (mPlayer->getState() == 0)
            mPlayer->activate();

        if (debugOutput){
            m_outFileDebug << simTime() << " [PLAYER] state: " << mPlayer->getRealState() << " started: " << mPlayer->playerStarted() << " @ " << mPlayer->getCurrentPlaybackPoint() << " : " << mPlayer->getCountChunkHit() << " - " << mPlayer->getCountChunkMiss() << endl;
            m_outFileDebug << simTime() << " [PLAYER] state2: " << mPlayer->getChunksHit() << " , " << mPlayer->getChunksMissed() << " , " << mPlayer->getChunksStalled() << " , " << mPlayer->getChunksSkipped() << endl;
        }
        scheduleAt(simTime() + param_ChunkScheduleInterval, timer_chunkScheduler);
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

    int stripe;
    IPvXAddress newParent;

    switch (csp->getPacketType()){
        case MTREEBONE_PARENT_REQUEST_RESPONSE: // handle response
            handleParentRequestResponse(src, check_and_cast<MTreeBoneParentRequestResponsePacket *> (pkt) );
            break;
        case MTREEBONE_SWITCH_POSITION_REQUEST: // handle request
            handleSwitchPositionRequest(src, check_and_cast<MTreeBonePeerSwitchPostionRequestPacket*> (pkt) );
            break;
        case MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE: // handle response;
            handleSwitchPositionResponse(src, check_and_cast<MTreeBonePeerSwitchPostionRequestResponsePacket*> (pkt) );
            break;
        case MTREEBONE_INFORM_NEW_PARENT:
            stripe = (check_and_cast<MTreeBonePeerInformNewParentPacket*> (pkt))->getStripe();

            newParent = (check_and_cast<MTreeBonePeerInformNewParentPacket*> (pkt))->getNewParent();
            if (!newParent.equals(m_Stripes[stripe].Parent))
                removeParent(stripe);

            m_Stripes[stripe].Parent = newParent;
            addNeighbor(m_Stripes[stripe].Parent, stripe);

            if (debugOutput)
                m_outFileDebug << simTime().str() << " [INFORM_PARENT] got new parent " << m_Stripes[stripe].Parent.str() << " for stripe " << stripe << endl;
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

void MTreeBonePeer::onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount){

    removePendingRequest(sequenceNumber);

    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);
    MTreeBonePeerInformation* info = getPeerInformation(src);
    EV << endl << " MTreeBonePeer::onNewChunk @ " << m_localAddress.str() << " -> " << sequenceNumber << " , " << (info == NULL) << endl << endl;

    if (debugOutput)
        m_outFileDebug << simTime().str() << " [DOWN] got new chunk " << sequenceNumber << " from " << src.str() << endl;

    if (info != NULL){
        info->chunksReceived++;
    }
    simulation.setContext(origContext);

    globalDown++;
    m_ChunksReceived++;

    int stripe = sequenceNumber % param_numStripes;
    MTreeBoneStats::theStats->chunkReceived(this, sequenceNumber, hopcount, m_Stripes[stripe].Parent.equals(src));

    MTreeBoneBase::onNewChunk(src, sequenceNumber, hopcount);
}

void MTreeBonePeer::onDuplicateChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    int stripe = sequenceNumber % param_numStripes;

    EV << endl << " MTreeBonePeer::onDuplicateChunk @ " << m_localAddress.str() << " -> " << sequenceNumber << " from " << src.str() << endl;
    if (debugOutput)
        m_outFileDebug << simTime().str() << " [DOWN] got duplicate chunk " << sequenceNumber << " from " << src.str() << " prob. via push" << m_Stripes[stripe].Parent.equals(src) << endl;

    MTreeBoneStats::theStats->chunkDuplicateReceived(this, src, sequenceNumber, m_Stripes[stripe].Parent.equals(src));
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
                    if (debugOutput)
                        m_outFileDebug << simTime().str() << " [NEIGHBOR] remove deserted peer(a): " << (*it).str() << " for stripe: " << i << endl;
                    MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
                    resp->setStripeNumber(i);
                    resp->setIsAccepted(false);
                    sendToDispatcher(resp, m_localPort, *it, m_destPort);

                    if (debugOutput)
                        m_outFileDebug << simTime().str() << " [NEIGHBOR] remove deserted peer(b): " << (*it).str() << " for stripe: " << i << endl;

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
            if (debugOutput)
                m_outFileDebug << simTime().str() << " [NEIGHBOR] too many neighbors for stripe " << i << " : " << m_Stripes[i].Neighbors.size() << endl;
        }
    }
}

IPvXAddress MTreeBonePeer::findBetterParent(int stripe){
    MTreeBonePeerInformation* info;

    int currentDistance = getPeerInformation( m_Stripes[stripe].Parent )->getDistance(stripe);

    // check neighbors
    genericList<IPvXAddress> possible;
    std::vector<IPvXAddress>::iterator it;
    for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
        info = getPeerInformation(*it);
        if (info == NULL) continue;

        if (    (info->getDistance(stripe) < currentDistance) &&
                (info->getDistance(stripe) >= 0) &&
                (info->getSequenceNumberEnd() > mPlayer->getCurrentPlaybackPoint() + m_ChunksPerSecond)
           )
        {
            if (debugOutput)
                m_outFileDebug << simTime().str() << " [PARENTSELECTION]findBetterParent_a adding possible " << (*it).str() << " distance " << info->getDistance(stripe) << endl;
            possible.addItem(*it);
        }
    }

    std::vector<IPvXAddress> gossipAddr = m_Gossiper->getKnownPeers();
    for (it = gossipAddr.begin(); it != gossipAddr.end(); it++){
        GossipUserData* userData = m_Gossiper->getPeerData(*it);
        if (userData == NULL) continue;

        MTreeBoneGossipData* data = check_and_cast<MTreeBoneGossipData*> (userData);
        if ( (!data->getIsBoneNode(stripe)) || (data->getDistance(stripe) >= currentDistance) || (data->getDistance(stripe) < 0)){
            delete userData;
            continue;
        }

        if (debugOutput)
            m_outFileDebug << simTime().str() << " [PARENTSELECTION]findBetterParent_b adding possible " << (*it).str() << " distance " << data->getDistance(stripe) << endl;
        possible.addItem(*it);
        delete userData;
    }

    possible.removeItem(m_localAddress);

    if (possible.size() <= 0)
        return IPvXAddress("0.0.0.0");

    return possible.at( intrand(possible.size()) );
}

IPvXAddress MTreeBonePeer::findSwapCandidate(int stripe){
    MTreeBonePeerInformation* info;

    IPvXAddress bestCandidate;
    int currentDistance = getPeerInformation( m_Stripes[stripe].Parent )->getDistance(stripe);

    // check neighbors
    std::vector<IPvXAddress>::iterator it;
    for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
        info = getPeerInformation(*it);
        if (info == NULL) continue;

        if (    (info->getDistance(stripe) < currentDistance) &&
                (info->getDistance(stripe) >= 0) &&
                (info->getNumChildren(stripe) < m_Stripes[stripe].Children.size())
                //(info->getSequenceNumberEnd() > mPlayer->getCurrentPlaybackPoint() + m_ChunksPerSecond) // shouldnt be important because we swap higher into the tree
           )
        {
            if (debugOutput)
                m_outFileDebug << simTime().str() << " [SWAPSELECTION]findSwapCandidate_a new best: " << (*it).str() << " distance " << info->getDistance(stripe) << endl;
            bestCandidate = (*it);
            currentDistance = info->getDistance(stripe);
        }
    }

    std::vector<IPvXAddress> gossipAddr = m_Gossiper->getKnownPeers();
    for (it = gossipAddr.begin(); it != gossipAddr.end(); it++){
        GossipUserData* userData = m_Gossiper->getPeerData(*it);
        if (userData == NULL) continue;

        MTreeBoneGossipData* data = check_and_cast<MTreeBoneGossipData*> (userData);
        if ( (!data->getIsBoneNode(stripe)) || (data->getDistance(stripe) >= currentDistance) || (data->getDistance(stripe) < 0) || (data->getNumChildren(stripe) >= m_Stripes[stripe].Children.size())){
            delete userData;
            continue;
        }

        if (debugOutput)
            m_outFileDebug << simTime().str() << " [SWAPSELECTION]findSwapCandidate_a new best: " << (*it).str() << " distance " << info->getDistance(stripe) << endl;

        bestCandidate = (*it);
        currentDistance = info->getDistance(stripe);

        delete userData;
    }

    return bestCandidate;
}

void MTreeBonePeer::checkParents(){

    IPvXAddress addr;
    MTreeBonePeerInformation* info;

    for (unsigned int stripe = 0; stripe < param_numStripes; stripe ++){
        if (m_Stripes[stripe].nextParentRequest > simTime()) // not waited long enough ...
            continue;

        if (!wantToBeBoneNode(stripe)) // we dont want to be a bone node?
            continue;

        if (m_Stripes[stripe].isBoneNode()){

            IPvXAddress SwapCandidate = findSwapCandidate(stripe);

            if (SwapCandidate.isUnspecified()){ // no usefull swap candidate ...
                // look if we can find a better parent ...
                IPvXAddress newParentSuggestion = findBetterParent(stripe);

                if (!newParentSuggestion.isUnspecified())
                    requestParentShip(stripe, newParentSuggestion);
            }else{
                if (debugOutput)
                    m_outFileDebug << simTime().str() << " [SWITCH] switching position with: " << SwapCandidate.str() << " for stripe: " << stripe << endl;

                MTreeBonePeerSwitchPostionRequestPacket* req = new MTreeBonePeerSwitchPostionRequestPacket();
                req->setStripe(stripe);
                req->setSuggestedNewParent(m_Stripes[stripe].Parent);
                sendToDispatcher(req, m_localPort, SwapCandidate, m_destPort);
            }
        }else{
            if (!m_Stripes[stripe].isBoneNode())
                while (m_Stripes[stripe].Children.size() > 0)
                    removeChild(stripe, m_Stripes[stripe].Children.at(0));

            for (unsigned int i = 0; i < m_Stripes[stripe].Neighbors.size(); i++){
                int rnd = (int)intrand(m_Stripes[stripe].Neighbors.size());
                addr = m_Stripes[stripe].Neighbors.at(rnd);

                info = getPeerInformation(addr);
                if ( (info == NULL) || (!info->isBoneNode(stripe)) ) // no peer information or peer is not a bone node?
                    continue;

                requestParentShip(stripe, addr);
                break;
            }
        }

        /*if (m_Stripes[stripe].isBoneNode()){ // we have a parent
            // but we can check if there is a better parent ...
            int currentDistance = getPeerInformation( m_Stripes[stripe].Parent )->getDistance(stripe);

            // check if we can switch with parent
            info = getPeerInformation(m_Stripes[stripe].Parent);
            if ((debugOutput) && ( m_Stripes[stripe].Children.size() > 1 ))
                m_outFileDebug << simTime().str() << " [DEBUG] children=" << info->getNumChildren(stripe) << "," << m_Stripes[stripe].Children.size() << endl;
            if ( (info->getDistance(stripe) > 0) && (m_Stripes[stripe].Children.size() > info->getNumChildren(stripe)) && (info->getNumChildren(stripe) > 0) ){
                if (debugOutput)
                    m_outFileDebug << simTime().str() << " [SWITCH] switching position with: " << m_Stripes[stripe].Parent.str() << " for stripe: " << stripe << endl;

                MTreeBonePeerSwitchPostionRequestPacket* req = new MTreeBonePeerSwitchPostionRequestPacket();
                req->setStripe(stripe);
                req->setSuggestedNewParent(m_Stripes[stripe].Parent);
                sendToDispatcher(req, m_localPort, m_Stripes[stripe].Parent, m_destPort);
                continue;
            }

            // check neighbors
            genericList<IPvXAddress> possible;
            std::vector<IPvXAddress>::iterator it;
            for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
                info = getPeerInformation(*it);
                if (info == NULL) continue;

                if ( (info->getDistance(stripe) < currentDistance) &&
                        (info->getDistance(stripe) >= 0) &&
                        (info->getSequenceNumberEnd() > mPlayer->getCurrentPlaybackPoint())
                   ){
                    if (debugOutput)
                        m_outFileDebug << simTime().str() << " [PARENTSELECTION]a adding possible " << (*it).str() << " distance " << info->getDistance(stripe) << endl;
                    possible.addItem(*it);
                }
            }

            std::vector<IPvXAddress> gossipAddr = m_Gossiper->getKnownPeers();
            // for switching position
            IPvXAddress switchTo = IPAddress("0.0.0.0");
            int switchDistance   = -1;

            for (it = gossipAddr.begin(); it != gossipAddr.end(); it++){
                GossipUserData* userData = m_Gossiper->getPeerData(*it);
                if (userData == NULL) continue;

                //m_outFileDebug << simTime().str() << " [DEBUG] new parent selection 1" << endl;
                MTreeBoneGossipData* data = check_and_cast<MTreeBoneGossipData*> (userData);
                //m_outFileDebug << simTime().str() << " [DEBUG] new parent selection 2" << endl;
                if ( (!data->getIsBoneNode(stripe)) || (data->getDistance(stripe) >= currentDistance) || (data->getDistance(stripe) < 0)){
                    delete userData;
                    continue;
                }

                //if ((data->getNumChildren(stripe) < m_Stripes[stripe].Children.size()) && ( data->getDistance(stripe) > 0 )){
                //    if ( (switchDistance < 0) || (data->getDistance(stripe) < switchDistance) ){
                 //       switchTo = *it;
                  //      switchDistance = data->getDistance(stripe);
                 //       switchCounter++;
                 //   }
                //}
                //m_outFileDebug << simTime().str() << " [DEBUG] new parent selection 3" << endl;
                if (debugOutput)
                    m_outFileDebug << simTime().str() << " [PARENTSELECTION]b adding possible " << (*it).str() << " distance " << data->getDistance(stripe) << endl;
                possible.addItem(*it);
                delete userData;
                //m_outFileDebug << simTime().str() << " [DEBUG] new parent selection 4" << endl;
            }

            possible.removeItem(m_localAddress);

            if ((switchDistance > 0) && (uniform(0,1) < 0.25)){
                if (debugOutput)
                    m_outFileDebug << simTime().str() << " [SWITCH] switching position with: " << switchTo.str() << " for stripe: " << stripe << endl;

                MTreeBonePeerSwitchPostionRequestPacket* req = new MTreeBonePeerSwitchPostionRequestPacket();
                req->setStripe(stripe);
                req->setSuggestedNewParent(m_Stripes[stripe].Parent);
                sendToDispatcher(req, m_localPort, switchTo, m_destPort);
            }else if (possible.size() > 0){ // no better parents here ...
                int pos = intrand(possible.size());
                IPvXAddress query = possible.at(pos);

                MTreeBoneParentRequestPacket* req = new MTreeBoneParentRequestPacket();
                req->setStripeNumber(stripe);
                req->setAbort(false);

                sendToDispatcher(req, m_localPort, query, m_destPort);
                m_Stripes[stripe].nextParentRequest = simTime() + 3;

                if (debugOutput)
                    m_outFileDebug << simTime().str() << " [PARENT] send request to " << query.str() << " for stripe: "<< req->getStripeNumber() << " _ isNeigbor? " << m_Stripes[stripe].Neighbors.containsItem(query) << endl;
            }

            continue;// Done with this stripe
        }else{*/ // not a bone node for this stripe
            /*if ((m_Stripes[stripe].Children.size() > 0) && (intrand(5) == 1)){ // but we have children
                removeChild(stripe, m_Stripes[stripe].Children.at(0)); // randomly drop the first child -> Intention: maybe the child can find a better parent than we can and later we may adapt the child as a parent ...
            }*/

        //}


    }
}

void MTreeBonePeer::handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp){

    if (debugOutput)
        m_outFileDebug << simTime().str() << " [PARENT] got response from " << src.str() << " for stripe: " << resp->getStripeNumber() << " status: "<< (resp->getIsAccepted() ? "accepted" : "denied") << endl;

    int stripe = resp->getStripeNumber();

    if (resp->getIsAccepted()){
        removeParent(stripe);
        m_Stripes[stripe].Parent = src;
        addNeighbor(src, stripe);
    }else{
        if (m_Stripes[stripe].Parent.equals(src))
            m_Stripes[stripe].Parent = IPvXAddress("0.0.0.0");
        if (m_Stripes[stripe].Parent.isUnspecified())
            m_Stripes[stripe].nextParentRequest = simTime();
    }
}

void MTreeBonePeer::requestParentShip(int stripe, IPvXAddress addr){
    MTreeBoneParentRequestPacket* parentRequest = new MTreeBoneParentRequestPacket();
    parentRequest->setStripeNumber(stripe);
    parentRequest->setAbort(false);
    parentRequest->setOwnHead( m_videoBuffer->getBufferEndSeqNum());

    sendToDispatcher(parentRequest, m_localPort, addr, m_destPort);
    m_Stripes[stripe].nextParentRequest = simTime() + 3;

    if (debugOutput)
        m_outFileDebug << simTime().str() << " [PARENT] send request to " << addr.str() << " for stripe: "<< stripe << endl;
}

void MTreeBonePeer::removeParent(int stripe){
    if (!m_Stripes[stripe].Parent.isUnspecified()){
        MTreeBoneParentRequestPacket* decline = new MTreeBoneParentRequestPacket();
        decline->setStripeNumber(stripe);
        decline->setAbort(true);
        sendToDispatcher(decline, m_localPort, m_Stripes[stripe].Parent, m_destPort);
        m_Stripes[stripe].Parent = IPvXAddress("0.0.0.0");
    }
}

void MTreeBonePeer::doChunkSchedule(){
    int head = getHeadSequenceNumber();
    //// adjust position ... TODO: check if this ok and how to alter player
    //if (
    //        (m_PlayerPosition < head - m_videoBuffer->getSize()*0.75)||
    //        ((m_PlayerPosition > head - m_videoBuffer->getSize()*0.25) && (head > m_videoBuffer->getSize() * 0.5))
    //){
    //    //m_outFileDebug << simTime().str() << " [PLAYING] adjust player position from " << m_PlayerPosition << " to " << (head - m_videoBuffer->getSize()/2) << endl;
   //     m_PlayerPosition = head - m_ChunksPerSecond * 2;// m_videoBuffer->getSize()/2;
    //}

    m_PlayerPosition = head - m_ChunksPerSecond * 5;// m_videoBuffer->getSize()/2;
    if (mPlayer->playerStarted())
        m_PlayerPosition = mPlayer->getCurrentPlaybackPoint();

    for (unsigned int i = 0; i < param_numStripes; i++)
        doChunkSchedule(i);

    //if (head > m_ChunksPerSecond * 2) // wait till head is atleast 2 seconds ahead
        m_PlayerPosition += param_ChunkScheduleInterval * m_ChunksPerSecond;
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
    //m_PlayerPosition = mPlayer->getPrefferedNextChunk();
    unsigned int oldestChunk = ((m_PlayerPosition % param_numStripes) == stripe)? m_PlayerPosition : m_PlayerPosition + (stripe - m_PlayerPosition % param_numStripes) + param_numStripes;
    unsigned int newestChunk = m_PlayerPosition + m_ChunksPerSecond * 5;//m_videoBuffer->getSize()/2;

    if (m_Stripes[stripe].isBoneNode()){
        //newestChunk = max(m_videoBuffer->getHeadReceivedSeqNum() - m_ChunksPerSecond, oldestChunk + m_ChunksPerSecond * 3);
        if (getPeerInformation(m_Stripes[stripe].Parent) != NULL){
            newestChunk = max(m_videoBuffer->getHeadReceivedSeqNum(), getPeerInformation(m_Stripes[stripe].Parent)->getSequenceNumberEnd());
            newestChunk = max(m_PlayerPosition + m_ChunksPerSecond, newestChunk - m_ChunksPerSecond);
            newestChunk = min(newestChunk, m_PlayerPosition + (int) (m_ChunksPerSecond * 2));
        }else   // shouldnt ever happen ....
            newestChunk = max(m_videoBuffer->getHeadReceivedSeqNum() - m_ChunksPerSecond, oldestChunk + m_ChunksPerSecond * 3);
    }
    if (debugOutput)
        m_outFileDebug << simTime().str() << " [doChunkSchedule] from " << oldestChunk << " to " << newestChunk << " for stripe: " << stripe << " isBoneNode? " << m_Stripes[stripe].isBoneNode() << endl;

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
                addr = getFirstPeerWithChunk(stripe, chunk); // get that peer
                if (debugOutput)
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

                std::vector<IPvXAddress>::iterator it;
                IPvXAddress best; double bestQuality = -1;
                for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
                    info = getPeerInformation(*it);

                    if ( (info->inBuffer(chunk)) && (getRequestCountInMap(requests, *it) < 15)){
                        double quality = ((numberOfRequests+1) - getRequestCountInMap(requests, *it));
                                quality *= info->getQuality();
                                quality /= (double)(numberOfRequests+1);
                        if ( (*it).equals(m_Stripes[stripe].Parent)) quality *= 0.25;

                        if (quality > bestQuality){
                            bestQuality = quality;
                            best = *it;
                        }
                    }
                }

                if (bestQuality > 0){
                    if (debugOutput)
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
            m_PendingRequests.insert(std::pair<int, SimTime>(iter->second.at(i), simTime() + param_ChunkRequestTimeout));
            pkt->setSequenceNumbers(i, iter->second.at(i));
        }

        info = getPeerInformation(iter->first);
        info->requestsSend += iter->second.size();

        if (debugOutput)
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

IPvXAddress MTreeBonePeer::getFirstPeerWithChunk(int stripe, int chunk){
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
        if (debugOutput)
            m_outFileDebug << simTime().str() << " [DOWN] timed out: " << sequenceNumber << endl;
        m_PendingRequests.erase(it);
        return false;
    }

    //m_outFileDebug << simTime().str() << " [requestIsPending] c " << sequenceNumber << endl;

    return true;
}

void MTreeBonePeer::removePendingRequest(unsigned int sequenceNumber){

    std::map<int, SimTime>::iterator it = m_PendingRequests.find(sequenceNumber);
    if (it == m_PendingRequests.end())
        return;

    m_PendingRequests.erase(it);
}

void MTreeBonePeer::handleSwitchPositionRequest(IPvXAddress src, MTreeBonePeerSwitchPostionRequestPacket* req){
    int stripe = req->getStripe();

    if (debugOutput)
        m_outFileDebug << simTime().str() << " [SWITCH] got request from" << src.str() << " with suggested new parent " << req->getSuggestedNewParent().str() << " for stripe " << stripe << endl;

    if (m_Stripes[stripe].Parent.isUnspecified()){
        if (debugOutput)
            m_outFileDebug << simTime().str() << " [SWITCH] DECLINED: NO PARENT" << endl;
        return;
    }

    MTreeBonePeerSwitchPostionRequestResponsePacket* resp = new MTreeBonePeerSwitchPostionRequestResponsePacket();
    resp->setStripe(stripe);
    resp->setNewParent(m_Stripes[stripe].Parent);

    // send switch
    MTreeBonePeerReplaceChildPacket* repChild = new MTreeBonePeerReplaceChildPacket();
    repChild->setNewChild(src);
    repChild->setStripe(stripe);
    sendToDispatcher(repChild, m_localPort, m_Stripes[stripe].Parent, m_destPort);

    //addNeighbor(src, stripe);
    //m_Stripes[stripe].Parent = src;//IPAddress("0.0.0.0");
    m_Stripes[stripe].Parent = IPAddress("0.0.0.0");

    //m_Stripes[stripe].Parent = req->getNewParent();
    //addNeighbor(req->getNewParent(), stripe);

    // remove sender as a child ...
    removeChild(stripe, src);

    sendToDispatcher(resp, m_localPort, src, m_destPort);

    // send a parent request to the suggested parent, if valid ...
    if (!req->getSuggestedNewParent().isUnspecified())
        requestParentShip(stripe, req->getSuggestedNewParent());

    //m_Stripes[stripe].nextParentRequest = 0;
    //checkParents();
}

void MTreeBonePeer::handleSwitchPositionResponse(IPvXAddress src, MTreeBonePeerSwitchPostionRequestResponsePacket* resp){
    int stripe = resp->getStripe();

    if (debugOutput)
        m_outFileDebug << simTime().str() << " [SWITCH] got response from " << src.str() << " with new parent " << resp->getNewParent().str() << " for stripe " << stripe << endl;

    IPvXAddress newParent = resp->getNewParent();
    if (!m_Stripes[stripe].Parent.equals(newParent))
        removeParent(stripe);

    addNeighbor(newParent, stripe);
    m_Stripes[stripe].Parent = newParent;
}

bool MTreeBonePeer::wantToBeBoneNode(int stripe){
    if (param_DisablePush) return false;

    //ev.getConfig()->getConfigValue("sim-time-limit");
    simtime_t stayedfor = simTime() - par("joinTime").doubleValue();
    simtime_t remaining = remaining.parse(ev.getConfig()->getConfigValue("sim-time-limit")) - simTime();//*2 for testing

    // test for early promotion -> this test can lead to a node wanting to be a bonenode->connecting->failed or disconnected-> not want to be a bonenode anymore ...
    //double test = (remaining-stayedfor).dbl();
    //test = 1 / test;

    /*
     *  k       x
        0.5     0.25
        0.6     0.264457737322019...
        0.8     0.291466380311610...
        1       0.3
        1.2     0.338932
        1.5     0.36981
        1.8     0.397388
        2       0.41421
     */

    double kResult = 0.3;
    if (MTreeBoneSettings::theSettings != NULL)
        kResult = MTreeBoneSettings::theSettings->getKResult();

    /*if ((remaining * kResult - stayedfor.dbl() + 1) > 0){
        double test;
        test = 1 / (remaining * kResult - stayedfor.dbl() + 1);
        if (dblrand() < test) return true;
    }*/

    return ( stayedfor > remaining * kResult);
    return !param_DisablePush;
}


void MTreeBonePeer::onPlayerStarted(){
    MTreeBoneStats::theStats->peerStartedPlayer(this);
    if (debugOutput){
        m_outFileDebug << simTime() << "[Player] started @ " << mPlayer->getCurrentPlaybackPoint() <<  endl;
        m_outFileDebug << m_videoBuffer->getBufferStartSeqNum() << "\t";

        for (SEQUENCE_NUMBER_T offset = 0; offset < 10; offset++)
            m_outFileDebug << " " << m_videoBuffer->inBuffer(mPlayer->getCurrentPlaybackPoint() + offset);

        m_outFileDebug << endl;
    }
}

void MTreeBonePeer::onChunksSkipped(SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition){
    MTreeBoneStats::theStats->debugChunksSkipped(m_localAddress, oldposition, newposition);
    if (debugOutput)
        m_outFileDebug << simTime() << "[Player] skipped chunks from " << oldposition << " to " << newposition << endl;
    MTreeBoneStats::theStats->onPlayerSkipped(this, oldposition, newposition);
};

void MTreeBonePeer::onChunkHit(SEQUENCE_NUMBER_T hit){ // hit equals played
    if (debugOutput)
        m_outFileDebug << simTime() << "[Player] hit " << hit << endl;
    MTreeBoneStats::theStats->chunkPlayed(this, hit);
};

void MTreeBonePeer::onChunkMiss(SEQUENCE_NUMBER_T miss){
    if (debugOutput)
        m_outFileDebug << simTime() << "[Player] miss " << miss << endl;
};
