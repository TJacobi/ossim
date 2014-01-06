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

MTreeBoneBase::MTreeBoneBase() {
    // TODO Auto-generated constructor stub
    m_LeftNetwork = true;
}

MTreeBoneBase::~MTreeBoneBase() {
    // TODO Auto-generated destructor stub
}

void MTreeBoneBase::handleMessage(cMessage *msg)
{
    // TODO: check if really needed?!
    if (msg->isSelfMessage())
    {
        handleTimerMessage(msg);
    }
}

void MTreeBoneBase::handleTimerMessage(cMessage *msg){

    if (msg == timer_joinNetwork){
        m_LeftNetwork = false;

        m_Gossiper->joinNetwork();

        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;

        onJoinedNetwork();
        //if (!isSource())
            //MTreeBoneStats::theStats->peerJoinedNetwork((MTreeBonePeer*) this);

        if (m_Mesh != NULL) m_Mesh->joinNetwork();
        if (m_Tree != NULL) m_Tree->joinNetwork();

        scheduleAt(simTime(), timer_checkNeighbors);
        scheduleAt(simTime() + 0.5, timer_chunkScheduler);
    }else
    if (msg == timer_leaveNetwork){
        m_LeftNetwork = true;
        m_Gossiper->leaveNetwork();

        cancelAndDelete(timer_leaveNetwork);    timer_leaveNetwork      = NULL;
        cancelAndDelete(timer_checkNeighbors);  timer_checkNeighbors    = NULL;
        cancelAndDelete(timer_chunkScheduler);  timer_chunkScheduler    = NULL;

        onLeavedNetwork();
        //if (!isSource())
            //MTreeBoneStats::theStats->peerLeavedNetwork((MTreeBonePeer*)this);

        if (m_Tree != NULL) m_Tree->leaveNetwork();
        //if (m_Mesh != NULL) m_Mesh->leaveNetwork();
    }else
    if (msg == timer_sendBufferMaps){
        if (m_Mesh != NULL) m_Mesh->timerBufferMaps();

        updateOwnGossipData(); // update gossip data

        scheduleAt(simTime() + MTreeBoneSettings::theSettings->getBufferMapIntervall(), timer_sendBufferMaps); // reschedule
    }else
    if (msg == timer_chunkScheduler){
        if (m_Mesh != NULL) m_Mesh->timerChunkScheduler();

        scheduleAt(simTime() + MTreeBoneSettings::theSettings->getChunkScheduleIntervall(), timer_chunkScheduler);
    }else
    if (msg == timer_checkNeighbors){
        onTimerCheckNeighbors();

        if (m_Mesh != NULL) m_Mesh->timerCheckNeighbors();
        if (m_Tree != NULL) m_Tree->timerCheckNeighbors();

        scheduleAt(simTime() + MTreeBoneSettings::theSettings->getCheckNeighborsIntervall(), timer_checkNeighbors);
    }
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

    // upload management
    param_MaxUploadFactor = par("MaxUploadFactor");
    m_UploadNextReset = 0;
    m_ChunksLeftForWindow = 0;
    m_FreeChunksLeftForWindow = 0;
    m_ReservedBandwidthFactor = 0;
        // Get the app settings
        temp = simulation.getModuleByPath("appSetting");
        AppSettingDonet* m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
        if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");
    m_ChunksPerSecond = MTreeBoneSettings::theSettings->getChunksPerSecond();
    m_Stripes = new MTreeBoneStripeInformation[MTreeBoneSettings::theSettings->getNumberOfStripes()];

    // read channel rate and adjust maxUploadFactor if necessary
    cDatarateChannel *channel = check_and_cast<cDatarateChannel *>(getParentModule()->getParentModule()->gate("pppg$o", 0)->getTransmissionChannel());
    double rate = channel->getDatarate() / m_appSetting->getVideoStreamBitRate();
    m_outFileDebug << simTime().str() << " [PARAMETER] MaxUploadFactor_a: " << param_MaxUploadFactor << endl;
    param_MaxUploadFactor = (param_MaxUploadFactor > rate) ? rate : param_MaxUploadFactor;
    m_outFileDebug << simTime().str() << " [PARAMETER] MaxUploadFactor: " << param_MaxUploadFactor << endl;

    //    checkFreeUploadListState();
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

    temp = getParentModule()->getModuleByRelativePath("meshmodule");
    if ( temp != NULL ){
        m_Mesh = check_and_cast<MTreeBoneMeshInterface *>(temp);
        m_Mesh->setBase(this);
        EV << "Binding to Forwarder is completed successfully" << endl;
    }

    temp = getParentModule()->getModuleByRelativePath("treemodule");
    if ( temp != NULL ){
        m_Tree = check_and_cast<MTreeBoneTreeInterface *>(temp);
        m_Tree->setBase(this);
        EV << "Binding to Forwarder is completed successfully" << endl;
    }


    m_Gossiper->addListener(this);

    if (m_Mesh) m_Mesh->doInit();
    if (m_Tree) m_Tree->doInit();


    // timers
    timer_chunkScheduler = new cMessage("MTreeBonePeer_CHUNK_SCHEDULER");
    timer_sendBufferMaps = new cMessage("MTreeBoneBase_SEND_BUFFERMAPS");
    timer_joinNetwork    = new cMessage("MTreeBonePeer_JOIN_NETWORK");
    timer_leaveNetwork   = new cMessage("MTreeBonePeer_LEAVE_NETWORK");
    timer_checkNeighbors = new cMessage("MTreeBonePeer_CHECK_NEIGHBORS");
    // schedule timers
    scheduleAt(simTime() + MTreeBoneSettings::theSettings->getBufferMapIntervall(), timer_sendBufferMaps);
    scheduleAt(par("joinTime").doubleValue(), timer_joinNetwork);
    if (par("stayDuration").doubleValue() > 0)
        scheduleAt(par("joinTime").doubleValue() + par("stayDuration").doubleValue(), timer_leaveNetwork);
}

/*
void MTreeBoneBase::checkFreeUploadListState(){
    m_FreeUploadListEnabled = ((param_MaxUploadFactor - m_ReservedBandwidthFactor)>= 1);
    if (!m_FreeUploadListEnabled){ // we dont have even enough bandwidth to support one peer ... -> just push chunks
        m_FreeUploadList.clear();
    }
}*/

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

void MTreeBoneBase::updateOwnGossipData(){
    MTreeBoneGossipData* data = new MTreeBoneGossipData(MTreeBoneSettings::theSettings->getNumberOfStripes());

    for (unsigned int stripe = 0; stripe < MTreeBoneSettings::theSettings->getNumberOfStripes(); stripe++){
        data->setIsBoneNode(stripe, isBoneNodeForStripe(stripe));
        data->setNumChildren(stripe, getReportedNumChildren(stripe));// m_Stripes[stripe].Children.size());
    }

    MTreeBonePeerInformationPacket* peerInfo = new MTreeBonePeerInformationPacket();
    peerInfo->setDistanceArraySize(MTreeBoneSettings::theSettings->getNumberOfStripes());
    peerInfo->setNumChildrenArraySize(MTreeBoneSettings::theSettings->getNumberOfStripes());
    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
        peerInfo->setDistance(i, getMyDistance(i));
        peerInfo->setNumChildren(i, getReportedNumChildren(i) );// m_Stripes[i].Children.size());
    }

    genericList<IPvXAddress> children; genericList<IPvXAddress>::iterator it;
    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++)
        for (it = m_Stripes[i].Children.begin(); it != m_Stripes[i].Children.end(); it++)
            children.addItem(*it);

    for (it = children.begin(); it != children.end(); it++)
        sendToDispatcher(peerInfo->dup(), m_localPort, *it, m_destPort);

    delete peerInfo;

    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++)
        m_outFileDebug << simTime().str() << " [DEBUG_GOSSIP] my Distance: stripe " << i << " = " << getMyDistance(i) << endl;

    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++)
        data->setDistance(i, getMyDistance(i));

    m_Gossiper->setOwnData(data);
    delete data;
}

int MTreeBoneBase::getMyDistance(int stripe){
    int ret = -1;

    // "push"-distance
    MTreeBonePeerInformation* info;
    //for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
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

MTreeBonePeerInformation* MTreeBoneBase::getPeerInformation(IPvXAddress addr){

    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it = m_Neighbors.find(addr);

    if (it == m_Neighbors.end()) return NULL;

    return it->second;
}

bool MTreeBoneBase::isBoneNodeForStripe(int stripe){
    return m_Stripes[stripe].isBoneNode();
}


bool MTreeBoneBase::sendChunk(SEQUENCE_NUMBER_T chunk, IPvXAddress dest, bool isPush){

    if (!getVideoBuffer()->inBuffer(chunk)) // cant send a chunk we dont have :&
        return false;

    if (isPush){ // assume that the coder did his job correctly ...
        m_forwarder->sendChunk(chunk, dest, m_destPort);
        MTreeBoneStats::theStats->chunkSendViaPush(m_localAddress, dest, chunk);

        return true;
    }

    if (m_UploadNextReset < simTime()){ // Bandwidth limit rest?
        m_UploadNextReset = simTime() + 1;

        m_ChunksLeftForWindow     = m_ChunksPerSecond * (param_MaxUploadFactor - m_ReservedBandwidthFactor);
        //m_FreeChunksLeftForWindow = (m_FreeUploadListEnabled) ? m_ChunksPerSecond : 0;
    }

    bool send = false;

    /*if (m_FreeUploadList.containsItem(chunk) && (m_FreeChunksLeftForWindow > 0)){
        m_FreeUploadList.removeItem(chunk);
        m_FreeChunksLeftForWindow--;
        send = true;
    }else if (m_ChunksLeftForWindow > m_FreeChunksLeftForWindow)*/
    if (m_ChunksLeftForWindow > 0)
        send = true;

    if (send){
        m_ChunksLeftForWindow--;
        m_forwarder->sendChunk(chunk, dest, m_destPort);
        MTreeBoneStats::theStats->chunkSendViaPull(m_localAddress, dest, chunk);

        return true;
    }
    return false;
}


SEQUENCE_NUMBER_T MTreeBoneBase::getHeadSequenceNumber(){
    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it;

    SEQUENCE_NUMBER_T ret = 0;

    for (it = m_Neighbors.begin(); it != m_Neighbors.end(); it++){
        ret = max(ret, (SEQUENCE_NUMBER_T)it->second->getSequenceNumberEnd());
    }

    return ret;
}

void MTreeBoneBase::addNeighbor(IPvXAddress addr, int stripe){
    if (addr.isUnspecified()) return;

    if (m_Stripes[stripe].Neighbors.containsItem(addr))
        return;

    m_outFileDebug << simTime() << " [NEIGHBOR] adding Neighbor " << addr.str() << " for stripe " << stripe << endl;

    m_Stripes[stripe].Neighbors.addItem(addr);
    MTreeBonePeerInformation* info = getPeerInformation(addr);
    if (info == NULL){
        info = new MTreeBonePeerInformation(MTreeBoneSettings::theSettings->getNumberOfStripes(), m_videoBuffer->getSize());
        m_Neighbors.insert(std::pair<IPvXAddress, MTreeBonePeerInformation*>(addr, info));
    }
}
void MTreeBoneBase::removeNeighbor(IPvXAddress addr, int stripe){
    m_outFileDebug << simTime() << " [NEIGHBOR] removing Neighbor " << addr.str() << " for stripe " << stripe << endl;

    m_Stripes[stripe].Neighbors.removeItem(addr);
    m_Stripes[stripe].Children.removeItem(addr);
    if (m_Stripes[stripe].Parent.equals(addr))
        m_Stripes[stripe].Parent = IPvXAddress("0.0.0.0");
    //checkFreeUploadListState();

    bool remove = true;
    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
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

SEQUENCE_NUMBER_T MTreeBoneBase::getCurrentPlaybackPoint(void){
    if ( (getPlayer() != NULL) && (getPlayer()->playerStarted()) )
        return getPlayer()->getCurrentPlaybackPoint();

    return getOwnHead();
}

SEQUENCE_NUMBER_T MTreeBoneBase::getOwnHead(){
    for (SEQUENCE_NUMBER_T pos = getVideoBuffer()->getBufferEndSeqNum(); pos >= getVideoBuffer()->getBufferStartSeqNum(); pos--)
        if (getVideoBuffer()->inBuffer(pos))
            return pos;

    return 0;
}

void MTreeBoneBase::sendPacket(cPacket *pkt, IPvXAddress dest) {
    Enter_Method_Silent();
    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);

    take(pkt);

    MTreeBoneStats::theStats->registerPacketSend( (MTreeBonePacketType) ((MTreeBonePacket*)pkt)->getPacketType() );

    sendToDispatcher(pkt, m_localPort, dest, m_destPort);

    // reset context
    simulation.setContext(origContext);
}

bool MTreeBoneBase::wantToBeBoneNode(int stripe){
    //if (param_DisablePush) return false;

    //ev.getConfig()->getConfigValue("sim-time-limit");
    simtime_t stayedfor = simTime() - getJoinTime();
    simtime_t remaining = remaining.parse(ev.getConfig()->getConfigValue("sim-time-limit")) - simTime();//*2 for testing

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

    // test for early promotion -> this test can lead to a node wanting to be a bonenode->connecting->failed or disconnected-> not want to be a bonenode anymore ...
    if ((remaining * kResult - stayedfor.dbl() + 1) > 0){
        double test;
        test = 1 / (remaining * kResult - stayedfor.dbl() + 1);
        if (dblrand() < test) return true;
    }

    return ( stayedfor > remaining * kResult);
}
