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

    if (debugOutput)
        m_outFileDebug.close();
}

void MTreeBoneBase::initBase() {
    cModule *temp;

    findNodeAddress();
    m_localPort = getLocalPort();
    m_destPort = getDestPort();

    // read parameters
    // partnership
    param_maxNOP     = par("maxNOP");
    param_desiredNOP = par("desiredNOP");;
    if (param_maxNOP < 1) param_maxNOP = 4;
    if (param_desiredNOP < 1) param_desiredNOP = (param_maxNOP / 2) < 1 ? 1 : param_maxNOP / 2;

    // upload management
    param_MaxUploadFactor = par("MaxUploadFactor");
    m_UploadNextReset = 0;
    m_ChunksLeftForWindow = 0;
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

    // timers
    timer_sendBufferMaps = new cMessage("MTreeBoneBase: TIMER_SEND_BUFFERMAPS");

    // schedule timers
    scheduleAt(simTime() + 1, timer_sendBufferMaps);

    // DEBUG
    debugOutput = true;//par("debugOutput").boolValue();
    if (debugOutput)
    {
        std::string filename = m_localAddress.str();
        EV << "DEBUG_OPEN_FILE: " << filename << endl;
        m_outFileDebug.open(filename.c_str(), std::fstream::out);
        m_outFileDebug << simTime().str() << " INIT " << endl;
    }
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
        for (unsigned int i = 0; i < param_numStripes; i++)
            EV << " Stripe " << i << ": " << ((m_Stripes[i].Parent.isUnspecified()) ? " NONE" : m_Stripes[i].Parent.str()) << endl;

        double avg = (double)m_ChunksUploaded * 100 / (m_ChunksUploaded + m_DebugOutput); //(double)m_ChunksUploaded*100 / (m_ChunksPerSecond * param_MaxUploadFactor * simTime());
        EV << endl << "  Upload left: " << m_ChunksLeftForWindow << " / " << (m_ChunksPerSecond * param_MaxUploadFactor) << " , approx avg utilization: " << avg << endl << endl;
        EV << "uploaded:" << m_ChunksUploaded << "  , denied: " << m_ChunksDenied << "  , +debug: " << m_DebugOutput << endl;
        m_videoBuffer->printStatus();

        std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator iter; // loop through all neighbors
        for(iter = m_Neighbors.begin();iter != m_Neighbors.end(); ++ iter)
        {
            sendToDispatcher(map->dup(), m_localPort, (*iter).first, m_destPort); // send a copy
        }

        delete map; // delete prepared packet

        updateOwnGossipData(); // update gossip data

        scheduleAt(simTime() + 0.5, timer_sendBufferMaps); // reschedule
    }
}

void MTreeBoneBase::processPacket(cPacket *pkt)
{
    MTreeBonePacket* csp = dynamic_cast<MTreeBonePacket*>(pkt);
    if (csp == NULL) return;

    IPvXAddress src = check_and_cast<DpControlInfo *>(pkt->getControlInfo())->getSrcAddr();
    MTreeBonePeerInformation* info;

    EV << "received packet: " << csp->getPacketType() << " @ " << m_localAddress.str() << endl;
    m_outFileDebug << simTime().str() << " [DEBUG] received packet " << csp->getPacketType() << endl;

    switch (csp->getPacketType()){
        case MTREEBONE_NEIGHBOR_REQUEST:
            processNeighborRequest(src, check_and_cast<MTreeBoneNeighborRequestPacket *> (pkt));
            break;
        case MTREEBONE_NEIGHBOR_REQUEST_RESPONSE:
            processNeighborRequestResponse(src, check_and_cast<MTreeBoneNeighborRequestResponsePacket *> (pkt));
            break;
        case MTREEBONE_CHUNK_REQUEST:
            handleChunkRequest(src, check_and_cast<MTreeBoneChunkRequestPacket *> (pkt));
            break;
        case MTREEBONE_CHUNK_REQUEST_LIST:
            handleChunkRequestList(src, check_and_cast<MTreeBoneChunkRequestListPacket *> (pkt));
            break;
        case MTREEBONE_BUFFER_MAP:
            info = getPeerInformation(src);
            if (info != NULL)
                info->updateFromBufferMap(check_and_cast<MTreeBoneBufferMapPacket *> (pkt));
            break;
        case MTREEBONE_PARENT_REQUEST:
            handleParentRequest(src, check_and_cast<MTreeBoneParentRequestPacket*> (pkt));
            break;
        default:
        {
            throw cException("Unrecognized packet type!");
        }
    }
}

void MTreeBoneBase::handleChunkRequest(IPvXAddress src, MTreeBoneChunkRequestPacket* pkt){
    if (m_UploadNextReset < simTime()){ // Bandwidth limit rest?
        m_UploadNextReset = simTime() + 1;
        m_DebugOutput += (int)m_ChunksLeftForWindow;

        m_ChunksLeftForWindow = m_ChunksPerSecond * ((param_MaxUploadFactor >= 1) ? param_MaxUploadFactor - 1 : param_MaxUploadFactor); // TODO: subtract children
    }

    EV << endl << endl << "MTreeBoneBase::handleChunkRequest @ " << m_localAddress.str() << " from " << src.str() << " -> " << pkt->getSequenceNumber() << endl << endl << endl;

    m_outFileDebug << simTime().str() << " [UP] handling chunk request from " << src.str() << " sequencenumber: " << pkt->getSequenceNumber() << endl;

    if ( (param_MaxUploadFactor >= 1) && (m_videoBuffer->isInBuffer(pkt->getSequenceNumber())) && (m_FreeUploadList.containsItem(pkt->getSequenceNumber())) ){
        m_ChunksUploaded++;
        m_FreeUploadList.removeItem(pkt->getSequenceNumber());

        m_outFileDebug << simTime().str() << " [UP] is in free upload list!" << endl;
        m_forwarder->sendChunk(pkt->getSequenceNumber(), src, m_destPort);
    }else if ((m_ChunksLeftForWindow > 0) && (m_videoBuffer->isInBuffer(pkt->getSequenceNumber()))){ // bandwidth left + do we have that chunk?
        m_ChunksLeftForWindow--;
        m_ChunksUploaded++;

        m_outFileDebug << simTime().str() << " [UP] sending from limit, left: " << m_ChunksLeftForWindow << endl;
        m_forwarder->sendChunk(pkt->getSequenceNumber(), src, m_destPort);
    }else{  // send deny
        MTreeBoneChunkDenyPacket* resp = new MTreeBoneChunkDenyPacket();
        resp->setSequenceNumber(pkt->getSequenceNumber());

        if (m_FreeUploadList.size() > 0){
            int rnd = (int)intrand(m_FreeUploadList.size());
            resp->setRequestThis(m_FreeUploadList.at(rnd));
        }else {
            resp->setRequestThis(0);
        }

        m_outFileDebug << simTime().str() << " [UP] denied, limit left: " << m_ChunksLeftForWindow << " inBuffer: " << m_videoBuffer->isInBuffer(pkt->getSequenceNumber()) << endl;
        sendToDispatcher(resp, m_localPort, src, m_destPort);

        if (!m_videoBuffer->isInBuffer(pkt->getSequenceNumber()))
            EV << " handleChunkRequest: NOT IN BUFFER " << pkt->getSequenceNumber() << " , " << m_videoBuffer->getBufferStartSeqNum() << " - " << m_videoBuffer->getHeadReceivedSeqNum() << endl;

        m_ChunksDenied++;
    }
}

void MTreeBoneBase::handleParentRequest(IPvXAddress src, MTreeBoneParentRequestPacket* pkt){
    MTreeBoneParentRequestResponsePacket* resp = new MTreeBoneParentRequestResponsePacket();

    int stripe = pkt->getStripeNumber();

    resp->setStripeNumber(stripe);
    resp->setIsAccepted( ((param_MaxUploadFactor - m_Stripes[stripe].Children.size()) > 1) || (m_Stripes[stripe].Children.containsItem(src)) ); // same max number of children for all stripes TODO: think about making it 1 + x ?

    if ( (resp->getIsAccepted()) && (!(m_Stripes[stripe].Children.containsItem(src))) ){
        m_Stripes[stripe].Children.addItem(src);
        checkFreeUploadListState();
    }

    m_outFileDebug << simTime().str() << " [PARENTING] got request from " << src.str() << " status: "<< (resp->getIsAccepted() ? "accepted" : "denied") << endl;

    sendToDispatcher(resp, m_localPort, src, m_destPort);
}

MTreeBoneBufferMapPacket* MTreeBoneBase::prepareBufferMap(){
    MTreeBoneBufferMapPacket* ret = new MTreeBoneBufferMapPacket();

    // set bone state
    ret->setBoneNodeForStripeArraySize(param_numStripes);
    for (unsigned int i = 0; i < param_numStripes; i++)
        ret->setBoneNodeForStripe(i, m_Stripes[i].isBoneNode());

    // set buffermap
    ret->setBufferMapArraySize(m_videoBuffer->getSize());
    //for (unsigned int i = 0; i < ret->getBufferMapArraySize(); i++)
//        ret->setBufferMap(i, m_videoBuffer->inBuffer(i));
    for (int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getHeadReceivedSeqNum(); i++)
        ret->setBufferMap(i % ret->getBufferMapArraySize(), m_videoBuffer->inBuffer(i));
    ret->setSequenceNumberStart(m_videoBuffer->getBufferStartSeqNum());
    ret->setSequenceNumberEnd(m_videoBuffer->getHeadReceivedSeqNum());

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
    if (m_Stripes[stripe].Neighbors.containsItem(addr))
        return;

    m_Stripes[stripe].Neighbors.addItem(addr);
    MTreeBonePeerInformation* info = getPeerInformation(addr);
    if (info == NULL){
        info = new MTreeBonePeerInformation(param_numStripes, m_videoBuffer->getSize());
        m_Neighbors.insert(std::pair<IPvXAddress, MTreeBonePeerInformation*>(addr, info));
    }
}
void MTreeBoneBase::removeNeighbor(IPvXAddress addr, int stripe){
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

void MTreeBoneBase::onNewChunk(IPvXAddress src, int sequenceNumber){
    if (m_FreeUploadListEnabled)
        m_FreeUploadList.addItem(sequenceNumber);

    // takeover context, because we may send something
    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);

    int stripe = sequenceNumber % param_numStripes;
    std::vector<IPvXAddress>::iterator it;

    for ( it = m_Stripes[stripe].Children.begin(); it != m_Stripes[stripe].Children.end(); it++){
        m_ChunksUploaded++;

        m_outFileDebug << simTime().str() << " [UP] forwarding received chunk " << sequenceNumber << " to " << (*it).str() << endl;
        m_forwarder->sendChunk(sequenceNumber, *it, m_destPort);
    }

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

    data->setHeadChunk(m_videoBuffer->getHeadReceivedSeqNum());
    for (unsigned int stripe = 0; stripe < param_numStripes; stripe++){
        data->setIsBoneNode(stripe, m_Stripes[stripe].isBoneNode());
        data->setNumChildren(stripe, m_Stripes[stripe].Children.size());
    }
    /*GossipUserData* data2 = data->dup();
    GossipUserData* data3 = data2->dup();*/

    /*m_outFileDebug << "[DEBUG] " << data4->getHeadChunk() << endl; m_outFileDebug.flush();
    m_outFileDebug << "[DEBUG] " << data4->getIsBoneNode(0) << endl; m_outFileDebug.flush();
    m_outFileDebug << "[DEBUG] " << data4->getNumChildren(0) << endl; m_outFileDebug.flush();*/

    //m_Gossiper->setOwnData(data);
    delete data;
}

void MTreeBoneBase::handleChunkRequestList(IPvXAddress src, MTreeBoneChunkRequestListPacket* pkt){
    if (m_UploadNextReset < simTime()){ // Bandwidth limit rest?
        m_UploadNextReset = simTime() + 1;
        m_DebugOutput += (int)m_ChunksLeftForWindow;

        m_ChunksLeftForWindow = m_ChunksPerSecond * ((param_MaxUploadFactor >= 1) ? param_MaxUploadFactor - 1 : param_MaxUploadFactor); // TODO: subtract children
    }

    EV << endl << endl << "MTreeBoneBase::handleChunkRequestList @ " << m_localAddress.str() << " from " << src.str() << " -> " << pkt->getSequenceNumbersArraySize() << endl << endl << endl;

    m_outFileDebug << simTime().str() << " [UP] handling chunk request list from " << src.str() << " sequencenumber count: " << pkt->getSequenceNumbersArraySize() << endl;

    for (unsigned int i = 0; i < pkt->getSequenceNumbersArraySize(); i++){
        int seqNumber = pkt->getSequenceNumbers(i);
        m_outFileDebug << simTime().str() << " [UP] handling chunk request from " << src.str() << " sequencenumber: " << seqNumber << endl;

        if ( (param_MaxUploadFactor >= 1) && (m_videoBuffer->isInBuffer(seqNumber)) && (m_FreeUploadList.containsItem(seqNumber)) ){
            m_ChunksUploaded++;
            m_FreeUploadList.removeItem(seqNumber);

            m_outFileDebug << simTime().str() << " [UP] is in free upload list!" << endl;
            m_forwarder->sendChunk(seqNumber, src, m_destPort);
            globalUp++;
        }else if ((m_ChunksLeftForWindow > 0) && (m_videoBuffer->isInBuffer(seqNumber))){ // bandwidth left + do we have that chunk?
            m_ChunksLeftForWindow--;
            m_ChunksUploaded++;

            m_outFileDebug << simTime().str() << " [UP] sending from limit, left: " << m_ChunksLeftForWindow << endl;
            m_forwarder->sendChunk(seqNumber, src, m_destPort);
            globalUp++;
        }else{  // send deny
            /*MTreeBoneChunkDenyPacket* resp = new MTreeBoneChunkDenyPacket();
            resp->setSequenceNumber(seqNumber);

            if (m_FreeUploadList.size() > 0){
                int rnd = (int)intrand(m_FreeUploadList.size());
                resp->setRequestThis(m_FreeUploadList.at(rnd));
            }else {
                resp->setRequestThis(0);
            }*/

            m_outFileDebug << simTime().str() << " [UP] denied, limit left: " << m_ChunksLeftForWindow << " inBuffer: " << m_videoBuffer->isInBuffer(seqNumber) << endl;
            /*sendToDispatcher(resp, m_localPort, src, m_destPort);

            if (m_videoBuffer->isInBuffer(seqNumber))
                ;
            else
                EV << " handleChunkRequest: NOT IN BUFFER " << seqNumber << " , " << m_videoBuffer->getBufferStartSeqNum() << " - " << m_videoBuffer->getHeadReceivedSeqNum() << endl;
             */

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
