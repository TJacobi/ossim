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

MTreeBoneBase::MTreeBoneBase() {
    // TODO Auto-generated constructor stub

}

MTreeBoneBase::~MTreeBoneBase() {
    // TODO Auto-generated destructor stub
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
        case MTREEBONE_BUFFER_MAP:
            info = getPeerInformation(src);
            if (info != NULL)
                info->updateFromBufferMap(check_and_cast<MTreeBoneBufferMapPacket *> (pkt));
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

    if ( (param_MaxUploadFactor >= 1) && (m_videoBuffer->isInBuffer(pkt->getSequenceNumber())) && (m_FreeUploadList.containsItem(pkt->getSequenceNumber())) ){
        m_ChunksUploaded++;
        m_FreeUploadList.removeItem(pkt->getSequenceNumber());
        m_forwarder->sendChunk(pkt->getSequenceNumber(), src, m_destPort);
    }else if ((m_ChunksLeftForWindow > 0) && (m_videoBuffer->isInBuffer(pkt->getSequenceNumber()))){ // bandwidth left + do we have that chunk?
        m_ChunksLeftForWindow--;
        m_ChunksUploaded++;
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

        sendToDispatcher(resp, m_localPort, src, m_destPort);

        if (m_videoBuffer->isInBuffer(pkt->getSequenceNumber()))
            ;
        else
            EV << " handleChunkRequest: NOT IN BUFFER " << pkt->getSequenceNumber() << " , " << m_videoBuffer->getBufferStartSeqNum() << " - " << m_videoBuffer->getHeadReceivedSeqNum() << endl;

        m_ChunksDenied++;
    }
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
    for (unsigned int i = m_videoBuffer->getBufferStartSeqNum(); i <= m_videoBuffer->getHeadReceivedSeqNum(); i++)
        ret->setBufferMap(i % ret->getBufferMapArraySize(), m_videoBuffer->inBuffer(i));
    ret->setSequenceNumberStart(m_videoBuffer->getBufferStartSeqNum());
    ret->setSequenceNumberEnd(m_videoBuffer->getHeadReceivedSeqNum());

    return ret;
}


void MTreeBoneBase::processNeighborRequest(IPvXAddress src, MTreeBoneNeighborRequestPacket* pkt){
    if ((unsigned int)pkt->getStripeNumber() > param_numStripes) // some one sending mailformed packets ...
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

    sendToDispatcher(resp, m_localPort, src, m_destPort);
}

void MTreeBoneBase::processNeighborRequestResponse(IPvXAddress src, MTreeBoneNeighborRequestResponsePacket* pkt){
    if ((unsigned int)pkt->getStripeNumber() > param_numStripes) // some one sending mailformed packets ...
        return;

    int stripe = pkt->getStripeNumber();

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
    if (param_MaxUploadFactor >= 1)
        m_FreeUploadList.addItem(sequenceNumber);
}
