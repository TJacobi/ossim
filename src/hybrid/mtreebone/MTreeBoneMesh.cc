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

#include "MTreeBoneMesh.h"
#include "MTreeBoneStats.h"

Define_Module(MTreeBoneMesh);

MTreeBoneMesh::MTreeBoneMesh() {
    // TODO Auto-generated constructor stub
}

MTreeBoneMesh::~MTreeBoneMesh() {
    // TODO Auto-generated destructor stub
}


void MTreeBoneMesh::timerChunkScheduler(){
    if (m_Base->isSource()) return;

    m_Base->m_outFileDebug << "timerChunkScheduler_1" << endl; m_Base->m_outFileDebug.flush();
    doChunkSchedule();
    m_Base->m_outFileDebug << "timerChunkScheduler_2" << endl; m_Base->m_outFileDebug.flush();
    if (m_Base->getPlayer() != NULL)
        m_Base->getPlayer()->activate(); // make sure player is running
}

void MTreeBoneMesh::doChunkSchedule(){
    int head = m_Base->getHeadSequenceNumber();

    m_Base->m_outFileDebug << "doChunkSchedule_1: " << head << endl; m_Base->m_outFileDebug.flush();
    SEQUENCE_NUMBER_T PlayerPosition = m_Base->getCurrentPlaybackPoint(); //head - MTreeBoneSettings::theSettings->getChunksPerSecond() * 5;// m_videoBuffer->getSize()/2;
    if (PlayerPosition == 0)
        PlayerPosition = head - MTreeBoneSettings::theSettings->getChunksPerSecond() * 5;// m_videoBuffer->getSize()/2;
    //if ( (m_Base->getPlayer() != NULL) && (m_Base->getPlayer()->playerStarted()) )
        //PlayerPosition = m_Base->getCurrentPlaybackPoint();

    m_Base->m_outFileDebug << "doChunkSchedule_2: " << PlayerPosition << endl; m_Base->m_outFileDebug.flush();
    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++)
        doChunkSchedule(i, PlayerPosition );
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

void MTreeBoneMesh::doChunkSchedule(unsigned int stripe, SEQUENCE_NUMBER_T playerposition){
    //m_PlayerPosition = mPlayer->getPrefferedNextChunk();
    unsigned int oldestChunk = ((playerposition % MTreeBoneSettings::theSettings->getNumberOfStripes()) == stripe)? playerposition : playerposition + (stripe - playerposition % MTreeBoneSettings::theSettings->getNumberOfStripes()) + MTreeBoneSettings::theSettings->getNumberOfStripes();
    unsigned int newestChunk = playerposition + MTreeBoneSettings::theSettings->getChunksPerSecond() * 5;//m_videoBuffer->getSize()/2;

    m_Base->m_outFileDebug << "doChunkSchedule_stripe_a: " << oldestChunk << ", " << newestChunk << endl; m_Base->m_outFileDebug.flush();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);
    MTreeBonePeerInformation*   pInfo;

    if (pStripe->isBoneNode()){
        //newestChunk = max(m_videoBuffer->getHeadReceivedSeqNum() - MTreeBoneSettings::theSettings->getChunksPerSecond(), oldestChunk + MTreeBoneSettings::theSettings->getChunksPerSecond() * 3);
        pInfo = m_Base->getPeerInformation(pStripe->Parent);
        if (pInfo != NULL){
            newestChunk = max(m_Base->getVideoBuffer()->getHeadReceivedSeqNum(), (SEQUENCE_NUMBER_T) pInfo->getSequenceNumberEnd());
            newestChunk = max(playerposition + MTreeBoneSettings::theSettings->getChunksPerSecond(), newestChunk - MTreeBoneSettings::theSettings->getChunksPerSecond());
            unsigned int temp = playerposition + (int) (MTreeBoneSettings::theSettings->getChunksPerSecond() * 2);
            newestChunk = min(newestChunk, temp);
        }else   // shouldnt ever happen ....
            newestChunk = max(m_Base->getVideoBuffer()->getHeadReceivedSeqNum() - MTreeBoneSettings::theSettings->getChunksPerSecond(), oldestChunk + MTreeBoneSettings::theSettings->getChunksPerSecond() * 3);
    }
    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [doChunkSchedule] from " << oldestChunk << " to " << newestChunk << " for stripe: " << stripe << " isBoneNode? " << pStripe->isBoneNode() << endl;

    IPvXAddress addr;
    std::map<IPvXAddress, genericList<int> >* requests = new std::map<IPvXAddress, genericList<int> >();
    int numberOfRequests = 0;

    // loop through the first chunk we require until the last chunk we need at the moment
    // TODO: look for push AND critical regions!
    // 1. loop = look for chunks with only one neighbor owning them
    for (unsigned int chunk = oldestChunk; chunk < newestChunk; chunk++){
        if ( (!m_Base->getVideoBuffer()->inBuffer(chunk)) && (! requestIsPending(chunk) ) ) // we dont have that chunk + request is not pending
            if (getNumberOfPeersWithChunk(stripe, chunk) == 1){ // it only exists one peer who has that chunk
                addr = getFirstPeerWithChunk(stripe, chunk); // get that peer
                if (m_Base->debugOutput)
                    m_Base->m_outFileDebug << simTime().str() << " [DOWN] request chunk from single source " << chunk << " , " << addr.str() << endl;
                if (getRequestCountInMap(requests, addr) < 25){
                    addRequestToMap(requests, addr, chunk);       // add him to the request list
                    numberOfRequests++;
                }
            }
    }

    // 2. loop = look for chunks with only one neighbor owning them
    for (unsigned int chunk = oldestChunk; chunk < newestChunk; chunk++){
        if ( (!m_Base->getVideoBuffer()->inBuffer(chunk)) && (! requestIsPending(chunk) ) ) // we dont have that chunk + request is not pending
            if (getNumberOfPeersWithChunk(stripe, chunk) > 1){ // only if multiply copies exist ...

                std::vector<IPvXAddress>::iterator it;
                IPvXAddress best; double bestQuality = -1;
                for (it = pStripe->Neighbors.begin(); it != pStripe->Neighbors.end(); it++){
                    pInfo = m_Base->getPeerInformation(*it);

                    if ( (pInfo->inBuffer(chunk)) && (getRequestCountInMap(requests, *it) < 15)){
                        double quality = ((numberOfRequests+1) - getRequestCountInMap(requests, *it));
                                quality *= pInfo->getQuality();
                                quality /= (double)(numberOfRequests+1);
                        if ( (*it).equals(pStripe->Parent)) quality *= 0.25;

                        if (quality > bestQuality){
                            bestQuality = quality;
                            best = *it;
                        }
                    }
                }

                if (bestQuality > 0){
                    if (m_Base->debugOutput)
                        m_Base->m_outFileDebug << simTime().str() << " [DOWN] picking " << best.str() << " for chunk " << chunk << " quality: " << bestQuality << endl;
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
            m_PendingRequests.insert(std::pair<int, SimTime>(iter->second.at(i), simTime() + MTreeBoneSettings::theSettings->getChunkRequestTimeout()));
            pkt->setSequenceNumbers(i, iter->second.at(i));
        }

        pInfo = m_Base->getPeerInformation(iter->first);
        pInfo->requestsSend += iter->second.size();

        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [DOWN] sending request list to " << iter->first.str() << " count: " << iter->second.size() << endl;

        m_Base->sendPacket(pkt , iter->first);
    }

    delete requests;
}


int MTreeBoneMesh::getNumberOfPeersWithChunk(int stripe, int chunk){
    std::vector<IPvXAddress>::iterator it;
    MTreeBonePeerInformation* info;
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    int ret = 0;

    for (it = pStripe->Neighbors.begin(); it != pStripe->Neighbors.end(); it++){
        info = m_Base->getPeerInformation(*it);
        if (info->inBuffer(chunk))
            ret++;
    }

    return ret;
}

IPvXAddress MTreeBoneMesh::getFirstPeerWithChunk(int stripe, int chunk){
    std::vector<IPvXAddress>::iterator it;
    MTreeBonePeerInformation* info;
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    for (it = pStripe->Neighbors.begin(); it != pStripe->Neighbors.end(); it++){
        info = m_Base->getPeerInformation(*it);
        if (info->inBuffer(chunk))
            return (*it);
    }

    return IPvXAddress("0.0.0.0");
}

bool MTreeBoneMesh::requestIsPending(SEQUENCE_NUMBER_T sequenceNumber){
    //m_outFileDebug << simTime().str() << " [requestIsPending] a " << sequenceNumber << endl;
    // check if we already requested that chunk
    std::map<SEQUENCE_NUMBER_T, SimTime>::iterator it = m_PendingRequests.find(sequenceNumber);
    if (it == m_PendingRequests.end())
        return false;

    //m_outFileDebug << simTime().str() << " [requestIsPending] b " << sequenceNumber << " timeout: " << it->second << endl;

    if (it->second < simTime() ){ // timed out
        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [DOWN] timed out: " << sequenceNumber << endl;
        m_PendingRequests.erase(it);
        return false;
    }

    //m_outFileDebug << simTime().str() << " [requestIsPending] c " << sequenceNumber << endl;

    return true;
}

void MTreeBoneMesh::removePendingRequest(SEQUENCE_NUMBER_T sequenceNumber){

    std::map<SEQUENCE_NUMBER_T, SimTime>::iterator it = m_PendingRequests.find(sequenceNumber);
    if (it == m_PendingRequests.end())
        return;

    m_PendingRequests.erase(it);
}

void MTreeBoneMesh::timerCheckNeighbors(){

    EV << "TreeBonePeer::checkNeighbors() @ " << m_Base->getAddress().str() << endl;
    MTreeBonePeerInformation* info;
    MTreeBoneStripeInformation* pStripe;
    std::vector<IPvXAddress>::iterator it;

    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
        pStripe = m_Base->getStripe(i);
        EV << "  Stripe: " << i << " partners: " << pStripe->Neighbors.size() << endl;
        if (pStripe->Neighbors.size() >= MTreeBoneSettings::theSettings->getNumbersOfPartnersDesired()){ // has enough neighbors, check if we want to kick someone
            for (it = pStripe->Neighbors.begin(); it != pStripe->Neighbors.end(); it++){
                info = m_Base->getPeerInformation( *it );
                if ((info == NULL) || (info->isProbablyDesertedPeer())){
                    if (m_Base->debugOutput)
                        m_Base->m_outFileDebug << simTime().str() << " [NEIGHBOR] remove deserted peer(a): " << (*it).str() << " for stripe: " << i << endl;
                    MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
                    resp->setStripeNumber(i);
                    resp->setIsAccepted(false);
                    m_Base->sendPacket(resp, *it);

                    if (m_Base->debugOutput)
                        m_Base->m_outFileDebug << simTime().str() << " [NEIGHBOR] remove deserted peer(b): " << (*it).str() << " for stripe: " << i << endl;

                    m_Base->removeNeighbor( *it, i);
                    break;
                }
            }
        }

        if (pStripe->Neighbors.size() < MTreeBoneSettings::theSettings->getNumbersOfPartnersDesired()){ // stripe needs/wants more neighbors
            for (int j = 0; j < (MTreeBoneSettings::theSettings->getNumbersOfPartnersDesired() - pStripe->Neighbors.size()); j++){
                IPvXAddress addr = m_Base->getGossiper()->getRandomPeer( m_Base->getAddress() );
                if ((!addr.isUnspecified()) && (!pStripe->Neighbors.containsItem(addr))){
                    MTreeBoneNeighborRequestPacket* request = new MTreeBoneNeighborRequestPacket();
                    request->setStripeNumber(i);
                    request->setJoinTime( m_Base->getJoinTime().dbl() );

                    m_Base->sendPacket(request, addr);
                }
            }
        }else if(pStripe->Neighbors.size() > MTreeBoneSettings::theSettings->getNumbersOfPartnersMax()){ // too many neighbors? shouldnt be possible ..
            if (m_Base->debugOutput)
                m_Base->m_outFileDebug << simTime().str() << " [NEIGHBOR] too many neighbors for stripe " << i << " : " << pStripe->Neighbors.size() << endl;
        }
    }
};

void MTreeBoneMesh::handleMessage(cMessage *msg){
    if (msg->isSelfMessage())
    {
        //handleTimerMessage(msg);
    }
    else
    {
        processPacket(PK(msg));
        delete msg;
    }
}

void MTreeBoneMesh::processPacket(cPacket *pkt){
    if (m_Base->hasLeftNetwork())
        return;

    MTreeBonePacketMesh* csp = dynamic_cast<MTreeBonePacketMesh*>(pkt);
    if (csp == NULL) return;

    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();

    IPvXAddress newParent;
    MTreeBonePeerInformation* info;

    switch (csp->getPacketType()){
        case MTREEBONE_BUFFER_MAP:
            info = m_Base->getPeerInformation(src);
            if (info != NULL){
                MTreeBoneBufferMapPacket* pBufferMapPacket = check_and_cast<MTreeBoneBufferMapPacket *> (pkt);

                if ((!pBufferMapPacket->getTempYouAreMyParent()) && (m_Base->getStripe(0)->Children.containsItem(src)))
                        m_Base->m_outFileDebug << simTime().str() << " [DEBUG_PARENT_ERROR] have child but he doesnt have us as parent! " << src.str() << endl;
                info->updateFromBufferMap(pBufferMapPacket);
            }
            if (info != NULL)
                m_Base->m_outFileDebug << simTime().str() << " [DEBUG] head= " << info->getSequenceNumberEnd() << endl;
            break;
        case MTREEBONE_NEIGHBOR_REQUEST:
            processNeighborRequest(src, check_and_cast<MTreeBoneNeighborRequestPacket *> (pkt));
            break;
        case MTREEBONE_NEIGHBOR_REQUEST_RESPONSE:
            processNeighborRequestResponse(src, check_and_cast<MTreeBoneNeighborRequestResponsePacket *> (pkt));
            break;
        case MTREEBONE_CHUNK_REQUEST_LIST:
            handleChunkRequestList(src, check_and_cast<MTreeBoneChunkRequestListPacket *> (pkt));
            break;
        default:
        {
            //MTreeBoneBase::processPacket(pkt);

            break;
        }
    }

}

void MTreeBoneMesh::processNeighborRequest(IPvXAddress src, MTreeBoneNeighborRequestPacket* pkt){
    if (pkt->getStripeNumber() > MTreeBoneSettings::theSettings->getNumberOfStripes()) // some one sending mailformed packets ...
        return;

    MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
    int stripe = pkt->getStripeNumber();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    resp->setStripeNumber(stripe);

    // stripe full or already contains peer?
    if ((pStripe->Neighbors.size() >= MTreeBoneSettings::theSettings->getNumbersOfPartnersMax()) || (pStripe->Neighbors.containsItem(src)))
        resp->setIsAccepted(pStripe->Neighbors.containsItem(src));
    else{
        m_Base->addNeighbor(src, stripe);

        m_Base->getPeerInformation(src)->setJoinTime( pkt->getJoinTime() );
        m_Base->getPeerInformation(src)->setFirstSeenTime(  );

        resp->setIsAccepted(true);
    }

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [NeighborRequest] from "<< src.str() <<" for "<< pkt->getStripeNumber() <<" accepted: "<< resp->getIsAccepted() << ", stripe full? "<< (pStripe->Neighbors.size() >= MTreeBoneSettings::theSettings->getNumbersOfPartnersMax()) << endl;

    m_Base->sendPacket(resp, src);
    //sendToDispatcher(resp, m_localPort, src, m_destPort);
}

void MTreeBoneMesh::processNeighborRequestResponse(IPvXAddress src, MTreeBoneNeighborRequestResponsePacket* pkt){
    if (pkt->getStripeNumber() > MTreeBoneSettings::theSettings->getNumberOfStripes()) // some one sending mailformed packets ...
        return;

    int stripe = pkt->getStripeNumber();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [NeighborRequestResponse] from "<< src.str() <<" for "<< pkt->getStripeNumber() <<" accepted: "<< pkt->getIsAccepted() << ", stripe full? "<< (pStripe->Neighbors.size() >= MTreeBoneSettings::theSettings->getNumbersOfPartnersMax()) << endl;

    if (pkt->getIsAccepted()){
        // got enough neighbors in the mean time?
        if (pStripe->Neighbors.size() >= MTreeBoneSettings::theSettings->getNumbersOfPartnersMax()){
            MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
            resp->setStripeNumber(stripe);
            resp->setIsAccepted(false);
            m_Base->sendPacket(resp, src);
            //sendToDispatcher(resp, m_localPort, src, m_destPort);
        }else{
            m_Base->addNeighbor(src, stripe);
        }
    }else{
        m_Base->removeNeighbor(src, stripe);
    }
}

void MTreeBoneMesh::handleChunkRequestList(IPvXAddress src, MTreeBoneChunkRequestListPacket* pkt){
    //EV << endl << endl << "MTreeBoneBase::handleChunkRequestList @ " << m_Base->getAddress().str() << " from " << src.str() << " -> " << pkt->getSequenceNumbersArraySize() << endl << endl << endl;

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [UP] handling chunk request list from " << src.str() << " sequencenumber count: " << pkt->getSequenceNumbersArraySize() << endl;

    for (unsigned int i = 0; i < pkt->getSequenceNumbersArraySize(); i++){
        int seqNumber = pkt->getSequenceNumbers(i);
        if (m_Base->debugOutput)
                m_Base->m_outFileDebug << simTime().str() << " [UP] handling chunk request from " << src.str() << " sequencenumber: " << seqNumber << endl;

        //if (m_Base->sendChunk(seqNumber, src, false))
        bool result =  m_Base->sendChunk(seqNumber, src, false);

        if (m_Base->debugOutput){
            if (result)
                m_Base->m_outFileDebug << simTime().str() << " [UP] send chunk " << seqNumber << " to " << src.str() << endl;
            else
                m_Base->m_outFileDebug << simTime().str() << " [UP] NOT send chunk " << seqNumber << " to " << src.str() << endl;
        }
    }
}

void MTreeBoneMesh::timerBufferMaps(){
    //EV << "MTreeBoneBase:timer_sendBufferMaps @ " << m_Base->getAddress().str() << " -> " << m_Base->getAllNeighbors()->size() << endl;
    MTreeBoneBufferMapPacket* map = prepareBufferMap(); // prepare packet

    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
        if(m_Base->getStripe(i)->Neighbors.size() > MTreeBoneSettings::theSettings->getNumbersOfPartnersMax()){ // too many neighbors? meh!
            IPvXAddress addr = m_Base->getStripe(i)->Neighbors.getRandomItem();
            if ( (!m_Base->getStripe(i)->Parent.equals(addr)) && (!m_Base->getStripe(i)->Children.containsItem(addr)) ){ // do not remove children or parent
                MTreeBoneNeighborRequestResponsePacket* quit = new MTreeBoneNeighborRequestResponsePacket();
                quit->setStripeNumber(i);
                quit->setIsAccepted(false);

                m_Base->sendPacket(quit, addr);
                //sendToDispatcher(quit, m_localPort, addr, m_destPort);

                m_Base->removeNeighbor(addr, i);
            }
        }
    }

    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator iter; // loop through all neighbors
    for(iter = m_Base->getAllNeighbors()->begin();iter != m_Base->getAllNeighbors()->end(); ++ iter)
    {
        MTreeBoneBufferMapPacket* temp = map->dup();
        temp->setTempYouAreMyParent(m_Base->getStripe(0)->Parent.equals((*iter).first));

        m_Base->sendPacket(temp, (*iter).first );
        //sendToDispatcher(temp, m_localPort, (*iter).first, m_destPort); // send a copy
    }

    delete map; // delete prepared packet
}

MTreeBoneBufferMapPacket* MTreeBoneMesh::prepareBufferMap(){
    MTreeBoneBufferMapPacket* ret = new MTreeBoneBufferMapPacket();

    // set bone state
    ret->setBoneNodeForStripeArraySize(MTreeBoneSettings::theSettings->getNumberOfStripes());
    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++)
        ret->setBoneNodeForStripe(i, m_Base->isBoneNodeForStripe(i));

    // set buffermap
    ret->setBufferMapArraySize(m_Base->getVideoBuffer()->getSize());

    for (int i =m_Base->getVideoBuffer()->getBufferStartSeqNum(); i <= m_Base->getVideoBuffer()->getHeadReceivedSeqNum(); i++)
        ret->setBufferMap(i % ret->getBufferMapArraySize(), m_Base->getVideoBuffer()->inBuffer(i));
    ret->setSequenceNumberStart(m_Base->getVideoBuffer()->getBufferStartSeqNum());
    ret->setSequenceNumberEnd(m_Base->getVideoBuffer()->getHeadReceivedSeqNum());
    ret->setMissingChunks(m_Base->getVideoBuffer()->getNumberOfCurrentlyMissingChunks());

    return ret;
}
