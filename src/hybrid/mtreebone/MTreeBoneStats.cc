#include "MTreeBoneStats.h"

MTreeBoneStats* MTreeBoneStats::theStats = NULL;

Define_Module(MTreeBoneStats);

MTreeBoneStats::MTreeBoneStats() {
    // TODO Auto-generated constructor stub
    m_Src = NULL;

    m_Timer_Report = NULL;
    m_Timer_Report_Stats = NULL;

    m_PeerOutput.open("peeractivity.txt");
}

MTreeBoneStats::~MTreeBoneStats() {
    if (m_Timer_Report != NULL) cancelAndDelete(m_Timer_Report); m_Timer_Report = NULL;
    if (m_Timer_Report_Stats != NULL) cancelAndDelete(m_Timer_Report_Stats); m_Timer_Report_Stats = NULL;

    //m_PeerOutput.close();
}

void MTreeBoneStats::initialize(int stage){
    if (stage !=2)
        return;

    MTreeBoneStats::theStats = this;

    // Get the number of stripes
    cModule* temp = simulation.getModuleByPath("appSetting");
    AppSettingDonet* m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
    if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");
    m_Stripes = m_appSetting->getNumberOfStripes();

    // create directory
    std::string command = "mkdir \"stats\"";
    system(command.c_str());

    m_Timer_Report       = new cMessage("MTREEBONESTATS_TIMER_REPORT");
    m_Timer_Report_Stats = new cMessage("MTREEBONESTATS_TIMER_REPORT_STATS");

    // initialize stats
    mStats_Peers_Joined = mStats_Peers_Leaved = mStats_Peers_Current = mStats_Peers_MaxConcurrent = 0;
    mStats_Peers_Joined_t = mStats_Peers_Leaved_t = mStats_Peers_Current_t = mStats_Peers_MaxConcurrent_t = 0;
    mStats_StartUpDelay_Min = mStats_StartUpDelay_Min_t = -1;
    mStats_StartUpDelay_Max = mStats_StartUpDelay_Max_t = -1;
    mStats_StartUpDelay_Total = mStats_StartUpDelay_Total_t = mStats_StartUpDelay_Count = mStats_StartUpDelay_Count_t = 0;
    //mStats_Chunks = new cChunkStats( 0, 0);
    mStats_Last   = new cChunkStats( 0, 0);
    mStats_Chunks_Pushed = mStats_Chunks_Pulled = 0;
    mStats_Chunks_Pushed_t = mStats_Chunks_Pulled_t = 0;

    scheduleAt(simTime() + 1, m_Timer_Report);
    scheduleAt(simTime() + 60, m_Timer_Report_Stats);
}

//#include "MyDebugClass.h"
void MTreeBoneStats::finish(){
//    debugOut("MTreeBoneStats::finish()");
    cleanOldData(-1);
    printStats();
}

void MTreeBoneStats::handleMessage(cMessage *msg){
    if (!msg->isSelfMessage())
        return;

    if (msg == m_Timer_Report){

        std::string command = "mkdir \"stats/" + simTime().str() + "\"";
        system(command.c_str());

        for (int stripe = 0; stripe < m_Stripes; stripe++)
            doReportForStripe(stripe);

        scheduleAt(simTime() + 1, m_Timer_Report);
    }else if(msg == m_Timer_Report_Stats){
        cleanOldData();

        printStats();

        scheduleAt(simTime() + 60, m_Timer_Report_Stats);
    }
}

void MTreeBoneStats::doReportForStripe(int stripe){
    std::stringstream filename;
    std::ofstream m_Writer;
    // source report
        filename << "stats/" << simTime().str() << "/stripe." << stripe << ".sources";
        m_Writer.open(filename.str().c_str());
        if (m_Src != NULL)
            m_Writer << m_Src->getAddress().str() << " " << endl;
        m_Writer.flush(); m_Writer.close();
    //peers
        filename.str("");
        filename << "stats/" << simTime().str() << "/stripe." << stripe << ".parents";
        m_Writer.open(filename.str().c_str());

        genericList<MTreeBonePeer*>::iterator it;
        for (it = m_Peers.begin(); it != m_Peers.end(); it++)
            if (! (*it)->getParent(stripe).isUnspecified())
                m_Writer << (*it)->getAddress() << " " << (*it)->getParent(stripe).str() << " " << endl;

        m_Writer.flush(); m_Writer.close();
}

void MTreeBoneStats::peerJoinedNetwork(MTreeBonePeer* peer){
    mPeerjoinTime.insert(std::pair<MTreeBonePeer*, simtime_t>(peer, simTime()));
    m_PeerOutput << simTime() << " [PEER] joined Network: " << peer->getAddress() << endl;
    m_Peers.addItem(peer);
    mStats_Peers_Joined++;
    mStats_Peers_Current++;

    if (mStats_Peers_Current > mStats_Peers_MaxConcurrent )
        mStats_Peers_MaxConcurrent = mStats_Peers_Current;
}
void MTreeBoneStats::peerLeavedNetwork(MTreeBonePeer* peer){
    m_PeerOutput << simTime() << " [PEER] leaved Network: " << peer->getAddress() << endl;
    m_Peers.removeItem(peer);

    mStats_Peers_Leaved++;
    mStats_Peers_Current--;
}
void MTreeBoneStats::peerStartedPlayer(MTreeBonePeer* peer){
    m_PeerOutput << simTime() << " [PEER] started player: " << peer->getAddress();
    if (mPeerjoinTime.find( peer ) == mPeerjoinTime.end()){
        m_PeerOutput << simTime() << " [ERROR] JOIN TIME UNKNOWN " << endl;
        return;
    }
    simtime_t delay = (simTime() - mPeerjoinTime.find(peer)->second);

    if ( (delay < mStats_StartUpDelay_Min) || (mStats_StartUpDelay_Min < 0) )
        mStats_StartUpDelay_Min = delay;
    if ( (delay > mStats_StartUpDelay_Max) || (mStats_StartUpDelay_Max < 0) )
        mStats_StartUpDelay_Max = delay;
    mStats_StartUpDelay_Total += delay;
    mStats_StartUpDelay_Count++;

    if ( (delay < mStats_StartUpDelay_Min_t) || (mStats_StartUpDelay_Min_t < 0) )
        mStats_StartUpDelay_Min_t = delay;
    if ( (delay > mStats_StartUpDelay_Max_t) || (mStats_StartUpDelay_Max_t < 0) )
        mStats_StartUpDelay_Max_t = delay;
    mStats_StartUpDelay_Total_t += delay;
    mStats_StartUpDelay_Count_t++;

    m_PeerOutput << ", startup delay: "<< delay << endl;
}

void MTreeBoneStats::chunkGenerated(SEQUENCE_NUMBER_T chunknumber){
    mChunkStats.insert(std::pair<SEQUENCE_NUMBER_T, cChunkStats*>(chunknumber, new cChunkStats( simTime(), mStats_Peers_Current )) );
}

void MTreeBoneStats::chunkReceived(MTreeBonePeer* peer, SEQUENCE_NUMBER_T chunknumber){
    //if (mChunkStats.find(chunknumber) == NULL) return;
    if (mChunkStats.find( chunknumber ) == mChunkStats.end()) return;
    cChunkStats* stats = mChunkStats.find(chunknumber)->second;
    if (stats == NULL) return;

    simtime_t delay = (simTime() - stats->generated);

    if ( (delay < stats->minDelay) || (stats->minDelay < 0) )
        stats->minDelay = delay;
    if ( (delay > stats->maxDelay) || (stats->maxDelay < 0) )
        stats->maxDelay = delay;
    stats->totalDelay += delay;
    stats->received++;
}

void MTreeBoneStats::printStats(){



    // Peer stats
    mStats_Peers_Joined_t += mStats_Peers_Joined;
    mStats_Peers_Leaved_t += mStats_Peers_Leaved;
    mStats_Peers_Current_t = mStats_Peers_Current;
    mStats_Peers_MaxConcurrent_t = max(mStats_Peers_MaxConcurrent,mStats_Peers_MaxConcurrent_t);

    m_PeerOutput << " [STATS][PEERS][BLOCK]";
    m_PeerOutput << " Joined: " << mStats_Peers_Joined;
    m_PeerOutput << " Leaved: " << mStats_Peers_Leaved;
    m_PeerOutput << " Current: " << mStats_Peers_Current;
    m_PeerOutput << " Max_Concurrent: " << mStats_Peers_MaxConcurrent;
    m_PeerOutput << endl;
    m_PeerOutput << " [STATS][PEERS][TOTAL]";
    m_PeerOutput << " Joined: " << mStats_Peers_Joined_t;
    m_PeerOutput << " Leaved: " << mStats_Peers_Leaved_t;
    m_PeerOutput << " Current: " << mStats_Peers_Current_t;
    m_PeerOutput << " Max_Concurrent: " << mStats_Peers_MaxConcurrent_t;
    m_PeerOutput << endl;

    mStats_Peers_Joined = mStats_Peers_Leaved = mStats_Peers_MaxConcurrent = 0;

    // startup delay
    m_PeerOutput << " [STATS][STARTUP_DELAY][BLOCK]";
    m_PeerOutput << " Min: " << mStats_StartUpDelay_Min;
    m_PeerOutput << " Max: " << mStats_StartUpDelay_Max;
    m_PeerOutput << " Avg: " << (mStats_StartUpDelay_Total/mStats_StartUpDelay_Count);
    m_PeerOutput << " Players started: " << mStats_StartUpDelay_Count;
    m_PeerOutput << endl;
    m_PeerOutput << " [STATS][STARTUP_DELAY][TOTAL]";
    m_PeerOutput << " Min: " << mStats_StartUpDelay_Min_t;
    m_PeerOutput << " Max: " << mStats_StartUpDelay_Max_t;
    m_PeerOutput << " Avg: " << (mStats_StartUpDelay_Total_t/mStats_StartUpDelay_Count_t);
    m_PeerOutput << " Players started: " << mStats_StartUpDelay_Count_t;
    m_PeerOutput << endl;
    mStats_StartUpDelay_Min = mStats_StartUpDelay_Max = -1;
    mStats_StartUpDelay_Total = mStats_StartUpDelay_Count = 0;


    /*// total chunks stats
    m_PeerOutput << " [STATS][CHUNKS_TOTAL]";
    m_PeerOutput << " Min: " << mStats_Chunks->minDelay;
    m_PeerOutput << " Max: " << mStats_Chunks->maxDelay;
    m_PeerOutput << " Avg: " << (mStats_Chunks->totalDelay / mStats_Chunks->received);
    m_PeerOutput << " Received: " << mStats_Chunks->received;
    m_PeerOutput << endl;*/

    // block chunks stats
    m_PeerOutput << " [STATS][CHUNKS][BLOCK] since: " << mStats_Last->generated << " Size: " << mStats_Last->peersAtGenerations << " chunks";
    m_PeerOutput << " Min: " << mStats_Last->minDelay;
    m_PeerOutput << " Max: " << mStats_Last->maxDelay;
    m_PeerOutput << " Avg: " << (mStats_Last->totalDelay / mStats_Last->received);
    m_PeerOutput << " Received: " << mStats_Last->received;
    m_PeerOutput << endl;

    // push/pull
    mStats_Chunks_Pushed_t += mStats_Chunks_Pushed;
    mStats_Chunks_Pulled_t += mStats_Chunks_Pulled;

    m_PeerOutput << " [STATS][METHOD][BLOCK]";
    m_PeerOutput << " Pull: " << mStats_Chunks_Pulled;
    m_PeerOutput << " Push: " << mStats_Chunks_Pushed;
    if ((mStats_Chunks_Pushed+mStats_Chunks_Pulled) > 0){
        double result = mStats_Chunks_Pushed * 100;
        result /= (mStats_Chunks_Pushed+mStats_Chunks_Pulled);
        m_PeerOutput << " Push-Ratio: " << result << "%";
    }
    m_PeerOutput << endl;
    mStats_Chunks_Pushed = mStats_Chunks_Pulled = 0;

    m_PeerOutput << " [STATS][METHOD][TOTAL]";
    m_PeerOutput << " Pull: " << mStats_Chunks_Pulled_t;
    m_PeerOutput << " Push: " << mStats_Chunks_Pushed_t;
    if ((mStats_Chunks_Pushed_t+mStats_Chunks_Pulled_t) > 0){
        double result = mStats_Chunks_Pushed_t * 100;
        result /= (mStats_Chunks_Pushed_t+mStats_Chunks_Pulled_t);
        m_PeerOutput << " Push-Ratio: " << result << "%";
    }
    m_PeerOutput << endl;

    // Player Stats
    long player_hits = 0, player_misses = 0, player_stalls = 0, player_skipped = 0;
    genericList<MTreeBonePeer*>::iterator it = m_Peers.begin();
    while (it != m_Peers.end()){
        player_hits    += (*it)->getPlayer()->getChunksHit();
        player_misses  += (*it)->getPlayer()->getChunksMissed();
        player_stalls  += (*it)->getPlayer()->getChunksStalled();
        player_skipped += (*it)->getPlayer()->getChunksSkipped();

        it++;
    }
    m_PeerOutput << " [STATS][PLAYER][CURRENT]";
    m_PeerOutput << " Hits: " << player_hits;
    m_PeerOutput << " Misses: " << player_misses;
    m_PeerOutput << " Stalled: " << player_stalls;
    m_PeerOutput << " Skipped: " << player_skipped;
    m_PeerOutput << " @ " << m_Peers.size() << " Peers";
    m_PeerOutput << endl;




    delete mStats_Last;
    mStats_Last = new cChunkStats( simTime(), 0);
}

void MTreeBoneStats::printChunkStats(SEQUENCE_NUMBER_T chunknumber){

    // chunk state
    m_PeerOutput << " [STATS][CHUNK] [" << chunknumber << "]";
    cChunkStats* stats = mChunkStats.find(chunknumber)->second;
    m_PeerOutput << " Generated: " << stats->generated;
    m_PeerOutput << " Min: " << stats->minDelay;
    m_PeerOutput << " Max: " << stats->maxDelay;
    m_PeerOutput << " Avg: " << (stats->totalDelay / stats->received);
    m_PeerOutput << " Received: " << stats->received;
    m_PeerOutput << " PeersAtGeneration: " << stats->peersAtGenerations;
    m_PeerOutput << endl;

}

void MTreeBoneStats::cleanOldData(simtime_t thresh){

    simtime_t threshold = simTime() - thresh;
    std::map<SEQUENCE_NUMBER_T, cChunkStats*>::iterator it;

    /*for (int i = mChunkStats.size()-1; i >= 0; i--){
        it = mChunkStats.begin();
        it += i;
        if ( (*it).second->generated < threshold){
            printChunkStats( (*it).first );
            delete (*it).second;
            mChunkStats.erase( it );
        }
    }*/

    it = mChunkStats.begin();
    while (it != mChunkStats.end()){
    //for (it = mChunkStats.begin(); it != mChunkStats.end(); it++){
        if ( (*it).second->generated < threshold){
            printChunkStats( (*it).first );

            /*if ( ((*it).second->minDelay < mStats_Chunks->minDelay) || (mStats_Chunks->minDelay < 0) )
                mStats_Chunks->minDelay = (*it).second->minDelay;
            if ( ((*it).second->maxDelay > mStats_Chunks->maxDelay) || (mStats_Chunks->maxDelay < 0) )
                mStats_Chunks->maxDelay = (*it).second->maxDelay;
            mStats_Chunks->totalDelay += (*it).second->totalDelay;
            mStats_Chunks->received   += (*it).second->received;*/

            if ( ((*it).second->minDelay < mStats_Last->minDelay) || (mStats_Last->minDelay < 0) )
                mStats_Last->minDelay = (*it).second->minDelay;
            if ( ((*it).second->maxDelay > mStats_Last->maxDelay) || (mStats_Last->maxDelay < 0) )
                mStats_Last->maxDelay = (*it).second->maxDelay;
            mStats_Last->totalDelay += (*it).second->totalDelay;
            mStats_Last->received   += (*it).second->received;
            mStats_Last->peersAtGenerations++;

            delete (*it).second;
            mChunkStats.erase( it++ );

            if (mChunkStats.size() == 0) break;
        }else{
            it++;
        }
    }
}

void MTreeBoneStats::chunkSendViaPush(IPvXAddress src, IPvXAddress dst, SEQUENCE_NUMBER_T chunknumber){
    mStats_Chunks_Pushed++;
}
void MTreeBoneStats::chunkSendViaPull(IPvXAddress src, IPvXAddress dst, SEQUENCE_NUMBER_T chunknumber){
    mStats_Chunks_Pulled++;
}
