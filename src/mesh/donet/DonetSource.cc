//#include "VideoSource.h"

#include "IPv4InterfaceData.h"
#include "InterfaceTableAccess.h"

#include "DonetSource.h"
#include "DpControlInfo_m.h"
#include "MeshPeerStreamingPacket_m.h"

Define_Module(DonetSource);

DonetSource::DonetSource() {}
DonetSource::~DonetSource()
{
    if (timer_sendBufferMap  != NULL)
    {
       delete cancelEvent(timer_sendBufferMap);    timer_sendBufferMap = NULL;
    }

    if (timer_sendReport != NULL)
    {
       delete cancelEvent(timer_sendReport);       timer_sendReport = NULL;
    }
}

void DonetSource::initialize(int stage)
{
    if (stage != 3)
        return;

    bindToGlobalModule();
    bindToMeshModule();

    getAppSetting();
    readChannelRate();

    findNodeAddress();

    param_maxNOP = par("maxNOP");

    timer_sendBufferMap = new cMessage("MESH_SOURCE_TIMER_SEND_BUFFERMAP");
    timer_sendReport    = new cMessage("MESH_SOURCE_TIMER_SEND_REPORT");

    // -- Register itself to the Active Peer Table
    m_apTable->addSourceAddress(getNodeAddress(), param_maxNOP);

    // -------------------------------------------------------------------------
    // -------------------------------- Timers ---------------------------------
    // -------------------------------------------------------------------------
    // -- Schedule events
    scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);

    // -- Report Logged Statistic to global module
    // scheduleAt(getSimTimeLimit() - uniform(0.05, 0.95), timer_sendReport);

    // -- States
    m_state = MESH_JOIN_STATE_ACTIVE;

    sig_pRequestRecv = registerSignal("Signal_pRequestRecv");
    sig_pRejectSent = registerSignal("Signal_pRejectSent");

    sig_pRequestRecv_whileWaiting = registerSignal("Signal_pRequestRecv_whileWaiting");

    // --- For logging variables
    m_arrivalTime = -1.0;
    m_joinTime = -1.0;
    m_video_startTime = -1.0;
    m_head_videoStart = -1L;
    m_begin_videoStart = -1L;
    m_threshold_videoStart = m_bufferMapSize_chunk/2;
    m_nChunkRequestReceived = 0L;
    m_nChunkSent = 0L;

    // -------------------------------------------------------------------------
    // -------------------------------- WATCH ----------------------------------
    // -------------------------------------------------------------------------
    WATCH(m_localAddress);
    WATCH(m_localPort);
    WATCH(m_destPort);

    WATCH(param_interval_bufferMap);
    WATCH(param_upBw);
    WATCH(param_downBw);
    WATCH(param_bufferMapSize_second);
    WATCH(param_chunkSize);
    WATCH(param_videoStreamBitRate);
    WATCH(param_maxNOP);

    WATCH(m_videoStreamChunkRate);
    WATCH(m_bufferMapSize_chunk);
    WATCH(m_BufferMapPacketSize_bit);

    WATCH(m_appSetting);
    WATCH(m_apTable);
    WATCH(m_partnerList);
    WATCH(m_videoBuffer);
}

void DonetSource::finish()
{
    //    if (m_videoBuffer != NULL) delete m_videoBuffer;

    m_gstat->reportNumberOfPartner(m_partnerList->getSize());

/*
    Partnership p;
        p.address = getNodeAddress();
        p.arrivalTime = 0.0;
        p.joinTime = 0.0;
        p.nPartner = m_partnerList->getSize();
        p.video_startTime = -1.0;
        p.head_videoStart = -1;
        p.begin_videoStart = -1;
        p.threshold_videoStart = -1;
    m_meshOverlayObserver->writeToFile(p);
*/
    //reportStatus();
}

/**
 * ----------------
 * Helper functions
 * ----------------
 */

void DonetSource::handleTimerMessage(cMessage *msg)
{
    if (msg == timer_sendBufferMap)
    {
        m_videoBuffer->printStatus();

        sendBufferMap();
        scheduleAt(simTime() + param_interval_bufferMap, timer_sendBufferMap);

    }
    else if (msg == timer_sendReport)
    {
       handleTimerReport();
    }
}

/*
 * This function DELETE the message
 */
void DonetSource::processPacket(cPacket *pkt)
{
    Enter_Method("processPacket(pkt)");

    // -- Get the address of the source node of the Packet
    DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(pkt->getControlInfo());
    IPvXAddress sourceAddress = controlInfo->getSrcAddr();

    PeerStreamingPacket *appMsg = dynamic_cast<PeerStreamingPacket *>(pkt);

    if (appMsg == NULL)
        return;
    if (appMsg->getPacketGroup() != PACKET_GROUP_MESH_OVERLAY)
    {
        throw cException("Wrong packet type!");
    }

    MeshPeerStreamingPacket *meshMsg = dynamic_cast<MeshPeerStreamingPacket *>(appMsg);
    switch (meshMsg->getPacketType())
    {
    case MESH_PARTNERSHIP_REQUEST:
    {
        processPartnershipRequest(pkt);
        break;
    }
    case MESH_CHUNK_REQUEST:
    {
        processChunkRequest(pkt);
        break;
    }
    case MESH_BUFFER_MAP:
    {
        // Does NOTHING! Video Source does not process Buffer Map
        break;
    }
    default:
    {
        // Should be some errors happen
        EV << "Errors when receiving unexpected message!" ;
        break;
    }
    } // End of switch

    delete pkt;
}
