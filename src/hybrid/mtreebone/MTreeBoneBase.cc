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

#include "MTreeBoneBase.h"
#include "AppSettingDonet.h"
#include "MTreeBoneStats.h"

int MTreeBoneBase::globalUp = 0;
int MTreeBoneBase::globalDown = 0;

MTreeBoneBase::MTreeBoneBase() {
    // TODO Auto-generated constructor stub

}

MTreeBoneBase::~MTreeBoneBase() {
    // TODO Auto-generated destructor stub

    delete [] this->m_Stripes;

    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it;

    for (it = m_Neighbors.begin(); it != m_Neighbors.end(); it++)
        delete it->second;

    //m_videoBuffer->removeListener(this);
    //m_Gossiper->removeListener(this);

    if (debugOutput)
        m_outFileDebug.close();
}

void MTreeBoneBase::initBase() {

    findNodeAddress(); // needed first for filename for debug

    // DEBUG
    debugOutput = par("debugOutput").boolValue();
    if (debugOutput)
    {
        std::string filename = m_localAddress.str() + ".debug";
        EV << "DEBUG_OPEN_FILE: " << filename << endl;
        m_outFileDebug.open(filename.c_str(), std::fstream::out);
        m_outFileDebug << simTime().str() << " INIT " << endl;
    }

    cModule *temp;

    m_localPort = getLocalPort();
    m_destPort = getDestPort();

    // read parameters
    // partnership
    param_maxNOP     = par("maxNOP");
    param_desiredNOP = par("desiredNOP");
    param_BufferMapIntervall = par("BufferMapIntervall");
    if (param_maxNOP < 1) param_maxNOP = 4;
    if (param_desiredNOP < 1) param_desiredNOP = (param_maxNOP / 2) < 1 ? 1 : param_maxNOP / 2;

    // upload management
    param_MaxUploadFactor = par("MaxUploadFactor");
    m_UploadNextReset = 0;
    m_ChunksLeftForWindow = 0;
    m_FreeChunksLeftForWindow = 0;
        // Get the app settings
        temp = simulation.getModuleByPath("appSetting");
        AppSettingDonet* m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
        if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");
    m_ChunksPerSecond = m_appSetting->getVideoStreamChunkRate();
    m_ChunksUploaded = 0;
    m_ChunksDenied = 0;
    m_DebugOutput = 0;
    param_numStripes  = m_appSetting->getNumberOfStripes();
    m_Stripes = new MTreeBoneStripeInformation[param_numStripes];

    // read channel rate and adjust maxUploadFactor if necessary
    cDatarateChannel *channel = check_and_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());
    double rate = channel->getDatarate() / m_appSetting->getVideoStreamBitRate();
    param_MaxUploadFactor = (param_MaxUploadFactor > rate) ? rate : param_MaxUploadFactor;
    m_outFileDebug << simTime().str() << " [PARAMETER] MaxUploadFactor: " << param_MaxUploadFactor << endl;

    checkFreeUploadListState();
    // bind to Gossip protocol
    // this module is required to retrieve random peers and get their informations for partnerships and rearrangements
    temp = getParentModule()->getModuleByRelativePath("gossipProtocol");
    m_Gossiper = check_and_cast<GossipProtocolWithUserData *>(temp);
    EV << "Binding to GossipProtocol is completed successfully" << endl;

    temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);
    EV << "Binding to VideoBuffer is completed successfully" << endl;

    temp = getParentModule()->getModuleByRelativePath("forwarder");
    m_forwarder = check_and_cast<Forwarder *>(temp);
    EV << "Binding to Forwarder is completed successfully" << endl;


    m_videoBuffer->addListener(this);
    m_Gossiper->addListener(this);

    // timers
    timer_sendBufferMaps = new cMessage("MTreeBoneBase: TIMER_SEND_BUFFERMAPS");

    // schedule timers
    scheduleAt(simTime() + param_BufferMapIntervall, timer_sendBufferMaps);
}

void MTreeBoneBase::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
    else
    {
        processPacket(PK(msg));
        delete msg;
    }
}

void MTreeBoneBase::handleTimerMessage(cMessage *msg){

    if (msg == timer_sendBufferMaps){
        EV << "MTreeBoneBase:timer_sendBufferMaps @ " << m_localAddress.str() << " -> " << m_Neighbors.size() << endl;
        MTreeBoneBufferMapPacket* map = prepareBufferMap(); // prepare packet

        EV << "  -> " << map->getSequenceNumberStart() << " , " << map->getSequenceNumberEnd() << endl;
        EV << " GLOBAL: down: " << globalDown << " , up: " << globalUp << endl;
        EV << " Player position: " << m_PlayerPosition << endl;

        EV << " Parents for Stripes: " << endl;
        for (unsigned int i = 0; i < param_numStripes; i++){
            if(m_Stripes[i].Neighbors.size() > param_maxNOP){ // too many neighbors? meh!
                IPvXAddress addr = m_Stripes[i].Neighbors.getRandomItem();
                if ( (!m_Stripes[i].Parent.equals(addr)) && (!m_Stripes[i].Children.containsItem(addr)) ){ // do not remove children or parent
                    MTreeBoneNeighborRequestResponsePacket* quit = new MTreeBoneNeighborRequestResponsePacket();
                    quit->setStripeNumber(i);
                    quit->setIsAccepted(false);
                    sendToDispatcher(quit, m_localPort, addr, m_destPort);
                    removeNeighbor(addr, i);
                }
            }

            EV << " Stripe " << i << ": " << ((m_Stripes[i].Parent.isUnspecified()) ? " NONE" : m_Stripes[i].Parent.str()) << endl;
        }

        double avg = (double)m_ChunksUploaded * 100 / (m_ChunksUploaded + m_DebugOutput); //(double)m_ChunksUploaded*100 / (m_ChunksPerSecond * param_MaxUploadFactor * simTime());
        EV << endl << "  Upload left: " << m_ChunksLeftForWindow << " / " << (m_ChunksPerSecond * param_MaxUploadFactor) << " , approx avg utilization: " << avg << endl << endl;
        EV << "uploaded:" << m_ChunksUploaded << "  , denied: " << m_ChunksDenied << "  , +debug: " << m_DebugOutput << endl;
        m_videoBuffer->printStatus();

        m_outFileDebug << simTime() << " [VIDEO_BUFFER] state: " << m_videoBuffer->getBufferStartSeqNum() << " - " << m_videoBuffer->getBufferEndSeqNum() << " , missing: " << m_videoBuffer->getNumberOfCurrentlyMissingChunks() << endl;

        std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator iter; // loop through all neighbors
        for(iter = m_Neighbors.begin();iter != m_Neighbors.end(); ++ iter)
        {
            MTreeBoneBufferMapPacket* temp = map->dup();
            temp->setTempYouAreMyParent(m_Stripes[0].Parent.equals((*iter).first));
            sendToDispatcher(temp, m_localPort, (*iter).first, m_destPort); // send a copy
        }

        delete map; // delete prepared packet

        updateOwnGossipData(); // update gossip data

        scheduleAt(simTime() + param_BufferMapIntervall, timer_sendBufferMaps); // reschedule
    }
}

void MTreeBoneBase::processPacket(cPacket *pkt)
{
    MTreeBonePacket* csp = dynamic_cast<MTreeBonePacket*>(pkt);
    if (csp == NULL) return;

    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();
    MTreeBonePeerInformation* info;

    EV << "received packet: " << csp->getPacketType() << " @ " << m_localAddress.str() << endl;
    m_outFileDebug << simTime().str() << " [DEBUG] received packet " << csp->getPacketType() << " from " << src.str() << endl;

    switch (csp->getPacketType()){
        case MTREEBONE_NEIGHBOR_REQUEST:
            processNeighborRequest(src, check_and_cast<MTreeBoneNeighborRequestPacket *> (pkt));
            break;
        case MTREEBONE_NEIGHBOR_REQUEST_RESPONSE:
            processNeighborRequestResponse(src, check_and_cast<MTreeBoneNeighborRequestResponsePacket *> (pkt));
            break;
        case MTREEBONE_CHUNK_REQUEST_LIST:
            handleChunkRequestList(src, check_and_cast<MTreeBoneChunkRequestListPacket *> (pkt));
            break;
        case MTREEBONE_BUFFER_MAP:
            info = getPeerInformation(src);
            if (info != NULL){
                MTreeBoneBufferMapPacket* pBufferMapPacket = check_and_cast<MTreeBoneBufferMapPacket *> (pkt);

                if ((!pBufferMapPacket->getTempYouAreMyParent()) && (m_Stripes[0].Children.containsItem(src)))
                        m_outFileDebug << simTime().str() << " [DEBUG_PARENT_ERROR] have child but he doesnt have us as parent! " << src.str() << endl;
                info->updateFromBufferMap(pBufferMapPacket);
            }
            if (info != NULL)
                m_outFileDebug << simTime().str() << " [DEBUG] head= " << info->getSequenceNumberEnd() << endl;
            break;
        case MTREEBONE_PARENT_REQUEST:
            handleParentRequest(src, check_and_cast<MTreeBoneParentRequestPacket*> (pkt));
            break;
        case MTREEBONE_PEER_INFORMATION:
            info = getPeerInformation(src);
            if (info != NULL)
                for (unsigned int i = 0; i < param_numStripes; i++){
                    info->setDistance(i, (check_and_cast<MTreeBonePeerInformationPacket*> (pkt))->getDistance(i) );
                    info->setNumChildren(i, (check_and_cast<MTreeBonePeerInformationPacket*> (pkt))->getNumChildren(i) );
                }

            break;
        case MTREEBONE_REPLACE_CHILD:
            handleReplaceChild(src, check_and_cast<MTreeBonePeerReplaceChildPacket*> (pkt));
            break;
        case MTREEBONE_SWITCH_POSITION_REQUEST: // handle request
            // maybe a child send a request ... just ignore it :D
            break;
        default:
        {
            throw cException("Unrecognized packet type!");
        }
    }
}

void MTreeBoneBase::handleParentRequest(IPvXAddress src, MTreeBoneParentRequestPacket* pkt){
    int stripe = pkt->getStripeNumber();

    if (pkt->getAbort()){ // peer doesnt want to be our children anymore
        m_Stripes[stripe].Children.removeItem(src);
        m_outFileDebug << simTime().str() << " [PARENTING] got remove from " << src.str() << " for stripe: " << stripe << endl;
        return;
    }

    MTreeBoneParentRequestResponsePacket* resp = new MTreeBoneParentRequestResponsePacket();
    resp->setStripeNumber(stripe);

    // same max number of children for all stripes TODO: think about making it 1 + x ?
    bool accepted = ((param_MaxUploadFactor - m_Stripes[stripe].Children.size()) > 1) || (m_Stripes[stripe].Children.containsItem(src));
    accepted = accepted && isBoneNodeForStripe(stripe);
    accepted = accepted && (m_videoBuffer->getBufferEndSeqNum() >= pkt->getOwnHead()); // only accept children which are not ahead of us

    resp->setIsAccepted( accepted );

    if ( (resp->getIsAccepted()) && (!(m_Stripes[stripe].Children.containsItem(src))) ){
        addNeighbor(src, stripe);

        // set head in order to prevent duplicates
        MTreeBonePeerInformation* info = getPeerInformation(src);
        info->setSequenceNumberEnd(pkt->getOwnHead());

        m_Stripes[stripe].Children.addItem(src);
        checkFreeUploadListState();
    }

    m_outFileDebug << simTime().str() << " [PARENTING] got request from " << src.str() << " for stripe: " << resp->getStripeNumber() << " status: "<< (resp->getIsAccepted() ? "accepted" : "denied") << " children: " << m_Stripes[stripe].Children.size() << endl;

    sendToDispatcher(resp, m_localPort, src, m_destPort);
}

void MTreeBoneBase::removeChild(int stripe, IPvXAddress addr){

    m_outFileDebug << simTime().str() << " [CHILD] remove child " << addr.str() << " for stripe: " << stripe << endl;
    m_Stripes[stripe].Children.removeItem(addr);

    MTreeBoneParentRequestResponsePacket* resp = new MTreeBoneParentRequestResponsePacket();
    resp->setStripeNumber(stripe);
    resp->setIsAccepted(false);
    sendToDispatcher(resp, m_localPort, addr, m_destPort);
}

MTreeBoneBufferMapPacket* MTreeBoneBase::prepareBufferMap(){
    MTreeBoneBufferMapPacket* ret = new MTreeBoneBufferMapPacket();

    // set bone state
    ret->setBoneNodeForStripeArraySize(param_numStripes);
    for (unsigned int i = 0; i < param_numStripes; i++)
        ret->setBoneNodeForStripe(i, isBoneNodeForStripe(i));

    // set buffermap
    ret->setBufferMapArraySize(m_videoBuffer->getSize());

    for (int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getHeadReceivedSeqNum(); i++)
        ret->setBufferMap(i % ret->getBufferMapArraySize(), m_videoBuffer->inBuffer(i));
    ret->setSequenceNumberStart(m_videoBuffer->getBufferStartSeqNum());
    ret->setSequenceNumberEnd(m_videoBuffer->getHeadReceivedSeqNum());
    ret->setMissingChunks(m_videoBuffer->getNumberOfCurrentlyMissingChunks());

    return ret;
}


void MTreeBoneBase::processNeighborRequest(IPvXAddress src, MTreeBoneNeighborRequestPacket* pkt){
    if (pkt->getStripeNumber() > param_numStripes) // some one sending mailformed packets ...
        return;

    MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
    int stripe = pkt->getStripeNumber();
    resp->setStripeNumber(stripe);

    // stripe full or already contains peer?
    if ((m_Stripes[stripe].Neighbors.size() >= param_maxNOP) || (m_Stripes[stripe].Neighbors.containsItem(src)))
        resp->setIsAccepted(m_Stripes[stripe].Neighbors.containsItem(src));
    else{
        addNeighbor(src, stripe);
        resp->setIsAccepted(true);
    }

    m_outFileDebug << simTime().str() << " [NeighborRequest] from "<< src.str() <<" for "<< pkt->getStripeNumber() <<" accepted: "<< resp->getIsAccepted() << ", stripe full? "<< (m_Stripes[stripe].Neighbors.size() >= param_maxNOP) << endl;

    sendToDispatcher(resp, m_localPort, src, m_destPort);
}

void MTreeBoneBase::processNeighborRequestResponse(IPvXAddress src, MTreeBoneNeighborRequestResponsePacket* pkt){
    if (pkt->getStripeNumber() > param_numStripes) // some one sending mailformed packets ...
        return;

    int stripe = pkt->getStripeNumber();

    m_outFileDebug << simTime().str() << " [NeighborRequestResponse] from "<< src.str() <<" for "<< pkt->getStripeNumber() <<" accepted: "<< pkt->getIsAccepted() << ", stripe full? "<< (m_Stripes[stripe].Neighbors.size() >= param_maxNOP) << endl;

    if (pkt->getIsAccepted()){
        // got enough neighbors in the mean time?
        if (m_Stripes[stripe].Neighbors.size() >= param_maxNOP){
            MTreeBoneNeighborRequestResponsePacket* resp = new MTreeBoneNeighborRequestResponsePacket();
            resp->setStripeNumber(stripe);
            resp->setIsAccepted(false);
            sendToDispatcher(resp, m_localPort, src, m_destPort);
        }else{
            addNeighbor(src, stripe);
        }
    }else{
        removeNeighbor(src, stripe);
    }
}

void MTreeBoneBase::addNeighbor(IPvXAddress addr, int stripe){
    if (addr.isUnspecified()) return;

    if (m_Stripes[stripe].Neighbors.containsItem(addr))
        return;

    m_outFileDebug << simTime() << " [NEIGHBOR] adding Neighbor " << addr.str() << " for stripe " << stripe << endl;

    m_Stripes[stripe].Neighbors.addItem(addr);
    MTreeBonePeerInformation* info = getPeerInformation(addr);
    if (info == NULL){
        info = new MTreeBonePeerInformation(param_numStripes, m_videoBuffer->getSize());
        m_Neighbors.insert(std::pair<IPvXAddress, MTreeBonePeerInformation*>(addr, info));
    }
}
void MTreeBoneBase::removeNeighbor(IPvXAddress addr, int stripe){
    m_outFileDebug << simTime() << " [NEIGHBOR] removing Neighbor " << addr.str() << " for stripe " << stripe << endl;

    m_Stripes[stripe].Neighbors.removeItem(addr);
    m_Stripes[stripe].Children.removeItem(addr);
    if (m_Stripes[stripe].Parent.equals(addr))
        m_Stripes[stripe].Parent = IPvXAddress("0.0.0.0");
    checkFreeUploadListState();

    bool remove = true;
    for (unsigned int i = 0; i < param_numStripes; i++){
        if (m_Stripes[i].Neighbors.containsItem(addr)){
            remove = false;
            break;
        }
    }

    if (remove){
        std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it = m_Neighbors.find(addr);

        if (it != m_Neighbors.end()){
            delete it->second;
            m_Neighbors.erase(it);
        }
    }
}

MTreeBonePeerInformation* MTreeBoneBase::getPeerInformation(IPvXAddress addr){

    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it = m_Neighbors.find(addr);

    if (it == m_Neighbors.end()) return NULL;

    return it->second;
}

void MTreeBoneBase::onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    // takeover context, because we may send something
    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);

    int stripe = sequenceNumber % param_numStripes;
    std::vector<IPvXAddress>::iterator it;

    bool count = false;

    MTreeBonePeerInformation* info;

    for ( it = m_Stripes[stripe].Children.begin(); it != m_Stripes[stripe].Children.end(); it++){

        info = getPeerInformation((*it));
        if ( (info != NULL) && (info->getSequenceNumberEnd() >= sequenceNumber))    // assume peer already has the chunk if his head is before this chunk
            continue;

        m_ChunksUploaded++;
        m_ChunksLeftForWindow--;

        MTreeBoneStats::theStats->chunkSendViaPush(m_localAddress, (*it), sequenceNumber);

        m_outFileDebug << simTime().str() << " [UP] forwarding received chunk " << sequenceNumber << " to " << (*it).str() << endl;
        m_forwarder->sendChunk(sequenceNumber, *it, m_destPort);
        count = true;
    }

    if (count)
        m_FreeChunksLeftForWindow--; //since we are forwarding a "unique" chunk
    else if (m_FreeUploadListEnabled) // add it to the list
        m_FreeUploadList.addItem(sequenceNumber);

    // reset context
    simulation.setContext(origContext);
}

void MTreeBoneBase::checkFreeUploadListState(){
    m_FreeUploadListEnabled = (param_MaxUploadFactor >= 1);
    if (!m_FreeUploadListEnabled) // we dont have even enough bandwidth to support one peer ... -> just push chunks
        return;

    double children = 0;
    for (unsigned int stripe = 0; stripe < param_numStripes; stripe++){
        children += m_Stripes[stripe].Children.size();
    }
    children = children / param_numStripes;

    m_FreeUploadListEnabled = ((param_MaxUploadFactor - children) >= 1);
}

void MTreeBoneBase::updateOwnGossipData(){
    MTreeBoneGossipData* data = new MTreeBoneGossipData(param_numStripes);

    for (unsigned int stripe = 0; stripe < param_numStripes; stripe++){
        data->setIsBoneNode(stripe, isBoneNodeForStripe(stripe));
        data->setNumChildren(stripe, m_Stripes[stripe].Children.size());
    }

    MTreeBonePeerInformationPacket* peerInfo = new MTreeBonePeerInformationPacket();
    peerInfo->setDistanceArraySize(param_numStripes);
    peerInfo->setNumChildrenArraySize(param_numStripes);
    for (unsigned int i = 0; i < param_numStripes; i++){
        peerInfo->setDistance(i, getMyDistance(i));
        peerInfo->setNumChildren(i, m_Stripes[i].Children.size());
    }

    genericList<IPvXAddress> children; genericList<IPvXAddress>::iterator it;
    for (unsigned int i = 0; i < param_numStripes; i++)
        for (it = m_Stripes[i].Children.begin(); it != m_Stripes[i].Children.end(); it++)
            children.addItem(*it);

    for (it = children.begin(); it != children.end(); it++)
        sendToDispatcher(peerInfo->dup(), m_localPort, *it, m_destPort);

    delete peerInfo;

    for (unsigned int i = 0; i < param_numStripes; i++)
        m_outFileDebug << simTime().str() << " [DEBUG_GOSSIP] my Distance: stripe " << i << " = " << getMyDistance(i) << endl;

    for (unsigned int i = 0; i < param_numStripes; i++)
        data->setDistance(i, getMyDistance(i));

    m_Gossiper->setOwnData(data);
    delete data;
}

void MTreeBoneBase::handleChunkRequestList(IPvXAddress src, MTreeBoneChunkRequestListPacket* pkt){
    if (m_UploadNextReset < simTime()){ // Bandwidth limit rest?
        m_UploadNextReset = simTime() + 1;
        m_DebugOutput += (int)m_ChunksLeftForWindow;

        m_ChunksLeftForWindow     = m_ChunksPerSecond * param_MaxUploadFactor;// ((param_MaxUploadFactor >= 1) ? param_MaxUploadFactor - 1 : param_MaxUploadFactor); // TODO: subtract children
        m_FreeChunksLeftForWindow = (param_MaxUploadFactor >= 1) ? m_ChunksPerSecond : 0;
    }

    EV << endl << endl << "MTreeBoneBase::handleChunkRequestList @ " << m_localAddress.str() << " from " << src.str() << " -> " << pkt->getSequenceNumbersArraySize() << endl << endl << endl;

    m_outFileDebug << simTime().str() << " [UP] handling chunk request list from " << src.str() << " sequencenumber count: " << pkt->getSequenceNumbersArraySize() << endl;

    for (unsigned int i = 0; i < pkt->getSequenceNumbersArraySize(); i++){
        int seqNumber = pkt->getSequenceNumbers(i);
        m_outFileDebug << simTime().str() << " [UP] handling chunk request from " << src.str() << " sequencenumber: " << seqNumber << endl;

        // upload left + in Buffer?
        if ( (m_ChunksLeftForWindow > 0) && (m_videoBuffer->isInBuffer(seqNumber)) ){
            //m_FreeChunksLeftForWindow
            if ( m_FreeUploadList.containsItem(seqNumber) ){
                m_ChunksLeftForWindow--;
                m_ChunksUploaded++;

                m_FreeUploadList.removeItem(seqNumber);
                m_FreeChunksLeftForWindow--;

                m_outFileDebug << simTime().str() << " [UP] is in free upload list!" << endl;
                m_forwarder->sendChunk(seqNumber, src, m_destPort);
                MTreeBoneStats::theStats->chunkSendViaPull(m_localAddress, src, seqNumber);
                globalUp++;
            }else if (m_ChunksLeftForWindow > m_FreeChunksLeftForWindow){
                m_ChunksLeftForWindow--;
                m_ChunksUploaded++;

                m_outFileDebug << simTime().str() << " [UP] sending from limit, left: " << m_ChunksLeftForWindow << ", " << m_FreeChunksLeftForWindow << endl;
                m_forwarder->sendChunk(seqNumber, src, m_destPort);
                MTreeBoneStats::theStats->chunkSendViaPull(m_localAddress, src, seqNumber);
                globalUp++;
            }
        }else{  // denied
            m_outFileDebug << simTime().str() << " [UP] denied, limit left: " << m_ChunksLeftForWindow << " inBuffer: " << m_videoBuffer->isInBuffer(seqNumber) << endl;

            m_ChunksDenied++;
        }
    }
}


int MTreeBoneBase::getHeadSequenceNumber(int stripe){
    std::vector<IPvXAddress>::iterator it;
    MTreeBonePeerInformation* info;

    int ret = 0;

    for (it = m_Stripes[stripe].Neighbors.begin(); it != m_Stripes[stripe].Neighbors.end(); it++){
        info = getPeerInformation(*it);
        int stripeChunk = info->getSequenceNumberEnd();
        stripeChunk = ((stripeChunk % param_numStripes) == stripe)? stripeChunk : stripeChunk + (stripe - stripeChunk % param_numStripes);
        ret = max(ret, stripeChunk);
    }

    return ret;
}

int MTreeBoneBase::getHeadSequenceNumber(){
    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it;

    int ret = 0;

    for (it = m_Neighbors.begin(); it != m_Neighbors.end(); it++){
        ret = max(ret, it->second->getSequenceNumberEnd());
    }

    return ret;
}

bool MTreeBoneBase::isBoneNodeForStripe(int stripe){
    return m_Stripes[stripe].isBoneNode();
}

int MTreeBoneBase::getMyDistance(int stripe){
    int ret = -1;

    // "push"-distance
    MTreeBonePeerInformation* info;
    //for (unsigned int i = 0; i < param_numStripes; i++){
    if (!m_Stripes[stripe].Parent.isUnspecified()){

        info = getPeerInformation(m_Stripes[stripe].Parent);
        if (info != NULL){
            int dist = info->getDistance(stripe);
            if ( (dist >= 0) &&
                 ( (dist < ret) || (ret < 0) ) )
                ret = dist;
        }
    }
    //}

    return (ret < 0) ? -1 : ret + 1;
}

void MTreeBoneBase::onGossipDataReceived(){
    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it;
    GossipUserData* data;

    m_outFileDebug << simTime().str() << " [DEBUG_GOSSIP] data received!" << endl;

    for (it = m_Neighbors.begin(); it != m_Neighbors.end(); it++){
        m_outFileDebug << simTime().str() << " [DEBUG_GOSSIP] data received! 1" << endl;
        data = m_Gossiper->getPeerData(it->first);

        if (data != NULL){
            m_outFileDebug << simTime().str() << " [DEBUG_GOSSIP] data received for " << it->first.str() << endl;
            it->second->setGossipData(check_and_cast<MTreeBoneGossipData*> (data) );
            delete data;
        }
    }
}

void MTreeBoneBase::handleReplaceChild(IPvXAddress src, MTreeBonePeerReplaceChildPacket* pkt){
    int stripe = pkt->getStripe();

    if (!m_Stripes[stripe].Children.containsItem(src))
        return; // a child wants to replace himself but he is not our child! oO

    m_outFileDebug << simTime().str() << " [SWITCH] replacing child " << src.str() << " with " << pkt->getNewChild().str() << " for stripe " << stripe << endl;

    // remove current child
    m_Stripes[stripe].Children.removeItem(src);

    // add new child
    addNeighbor(pkt->getNewChild(), stripe);
    m_Stripes[stripe].Children.addItem(pkt->getNewChild());
    checkFreeUploadListState();

    // inform new child
    MTreeBonePeerInformNewParentPacket* inform = new MTreeBonePeerInformNewParentPacket();
    inform->setStripe(stripe);
    inform->setNewParent(m_localAddress);
    sendToDispatcher(inform, m_localPort, pkt->getNewChild(), m_destPort);

    // send distance to new child
    MTreeBonePeerInformationPacket* peerInfo = new MTreeBonePeerInformationPacket();
    peerInfo->setDistanceArraySize(param_numStripes);
    peerInfo->setNumChildrenArraySize(param_numStripes);
    for (unsigned int i = 0; i < param_numStripes; i++){
        peerInfo->setDistance(i, getMyDistance(i));
        peerInfo->setNumChildren(i, m_Stripes[i].Children.size());
    }
    sendToDispatcher(peerInfo, m_localPort, pkt->getNewChild(), m_destPort);
}

void MTreeBoneBase::sendToDispatcher(MTreeBonePacket *pkt, int srcPort, const IPvXAddress& destAddr, int destPort){
    MTreeBoneStats::theStats->registerPacketSend((MTreeBonePacketType)pkt->getPacketType());
    CommBase::sendToDispatcher(pkt, srcPort, destAddr, destPort);
}
