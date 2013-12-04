#include "MTreeBoneStats.h"
#include "MTreeBonePacket_m.h"

MTreeBoneStats* MTreeBoneStats::theStats = NULL;

Define_Module(MTreeBoneStats);

MTreeBoneStats::MTreeBoneStats() {
    // TODO Auto-generated constructor stub
    m_Src = NULL;

    m_Timer_Report = NULL;
    m_Timer_Report_Stats = NULL;

    mStats_Packets = new long[MTREEBONE_PACKETS_MAX+1];
    for (int i = 0; i < MTREEBONE_PACKETS_MAX+1; i++)
        mStats_Packets[i] = 0;

}

MTreeBoneStats::~MTreeBoneStats() {
    if (m_Timer_Report != NULL) cancelAndDelete(m_Timer_Report); m_Timer_Report = NULL;
    if (m_Timer_Report_Stats != NULL) cancelAndDelete(m_Timer_Report_Stats); m_Timer_Report_Stats = NULL;

    delete[] mStats_Packets;
    //m_PeerOutput.close();
}

void MTreeBoneStats::initialize(int stage){

    if (stage == 3){
        cModule *temp;

        // Get the app settings
        temp = simulation.getModuleByPath("appSetting");
        AppSettingDonet* m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
        if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");

        m_PeerOutput << "Duration: " << ev.getConfig()->getConfigValue("sim-time-limit") << endl;
        m_PeerOutput << "Chunksize: " << m_appSetting->getChunkSize() << endl;
        if (MTreeBoneSettings::theSettings != NULL)
            m_PeerOutput << "K = " << MTreeBoneSettings::theSettings->getKResult() << endl;
    }


    if (stage !=2)
        return;

    MTreeBoneStats::theStats = this;

    // Get the number of stripes
    cModule* temp = simulation.getModuleByPath("appSetting");
    AppSettingDonet* m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
    if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");
    m_Stripes = m_appSetting->getNumberOfStripes();

    std::stringstream pTemp;
    pTemp << "runs/" << ev.getConfigEx()->getActiveRunNumber() << "/stats";
    mRootDirectory = pTemp.str() ;
    // create directory
    pTemp.str("");
    pTemp << "mkdir \"" << mRootDirectory << "\"";
    system(pTemp.str().c_str());

    std::stringstream filename;
    filename << mRootDirectory << "/peeractivity.txt";
    m_PeerOutput.open(filename.str().c_str());

    filename.str("");
    filename << mRootDirectory << "/startupdelay.txt";
    m_StartUpDelay.open(filename.str().c_str());

    filename.str("");
    filename << mRootDirectory << "/playbackdelay.txt";
    m_PlayBackDelay.open(filename.str().c_str());

    filename.str("");
    filename << mRootDirectory << "/chunkmisshits.txt";
    m_ChunkMissHits.open(filename.str().c_str());

    filename.str("");
    filename << mRootDirectory << "/receiverate.txt";
    m_PeerReceiveRate.open(filename.str().c_str());


    m_Timer_Report       = new cMessage("MTREEBONESTATS_TIMER_REPORT");
    m_Timer_Report_Stats = new cMessage("MTREEBONESTATS_TIMER_REPORT_STATS");

    // initialize stats
    mStats_Peers_Joined = mStats_Peers_Leaved = mStats_Peers_Current = mStats_Peers_MaxConcurrent = 0;
    mStats_Peers_Joined_t = mStats_Peers_Leaved_t = mStats_Peers_Current_t = mStats_Peers_MaxConcurrent_t = 0;
    mStats_StartUpDelay_Min = mStats_StartUpDelay_Min_t = -1;
    mStats_StartUpDelay_Max = mStats_StartUpDelay_Max_t = -1;
    mStats_StartUpDelay_Total = mStats_StartUpDelay_Total_t = mStats_StartUpDelay_Count = mStats_StartUpDelay_Count_t = 0;

    mStats_Player_Hits = mStats_Player_Miss = mStats_Player_Stall = mStats_Player_Skip = 0;
    //mStats_Chunks = new cChunkStats( 0, 0);
    mStats_Last   = new cChunkStats( 0, 0);
    mStats_Chunks_Pushed = mStats_Chunks_Pulled = 0;
    mStats_Chunks_Pushed_t = mStats_Chunks_Pulled_t = 0;

    mStats_PlayDelay_Count = 0;
    mStats_PlayDelay_SumAverage = 0;

    mStats_Hops_Push_Count = mStats_Hops_Push_Total = 0;
    mStats_Hops_Pull_Count = mStats_Hops_Pull_Total = 0;

    mStats_Duplicate_Push = mStats_Duplicate_Pull = 0;

    param_PeriodOutput = par("enablePeriodicalStructureOutput").boolValue();
    param_PeriodOutputIntervall = par("PeriodicalStructureOutputIntervall").doubleValue();

    scheduleAt(simTime() + param_PeriodOutputIntervall, m_Timer_Report);
    scheduleAt(simTime() + 60, m_Timer_Report_Stats);
}


void writeCDF(std::list<double> list, std::string filename){
    std::ofstream writestream; writestream.open(filename.c_str());

    list.sort();
    double count = 0; double current = -1;
    std::list<double>::iterator itDelay = list.begin();
    while (itDelay != list.end()){
        if ( (*itDelay) != current ){
            if (current >= 0)
                writestream << current << "\t" << (count / list.size()) << endl;
            current = (*itDelay);
        }
        count++;
        itDelay++;
    }
    if (current >= 0)
        writestream << current << "\t" << (count / list.size()) << endl;
    //writestream << "TOTAL: " << count << "\t" << list.size() << endl;
}


//#include "MyDebugClass.h"
void MTreeBoneStats::finish(){
//    debugOut("MTreeBoneStats::finish()");
    cleanOldData(-1);
    printStats();

    genericList<MTreeBonePeer*>::iterator it = m_Peers.begin();
    while (it != m_Peers.end()){
        m_ChunkMissHits << (*it)->getAddress().str() << "\t" << (*it)->getPlayer()->getChunksMissed() << "\t" << (*it)->getPlayer()->getChunksHit() << endl;
        double tmp = (*it)->getPlayer()->getChunksMissed() + (*it)->getPlayer()->getChunksHit();
        m_List_MissRatio.push_back( (double)(*it)->getPlayer()->getChunksMissed() * 100 / tmp );
        it++;
    }


    std::stringstream filename;
    filename << mRootDirectory << "/startupdelay_CDF.txt";
    writeCDF (m_List_StartUpDelay, filename.str());

    filename.str("");
    filename << mRootDirectory << "/playbackdelay_CDF.txt";
    writeCDF (m_List_PlayBackDelay, filename.str());

    filename.str("");
    filename << mRootDirectory << "/missratio_CDF.txt";
    writeCDF (m_List_MissRatio, filename.str());

}

void MTreeBoneStats::handleMessage(cMessage *msg){
    if (!msg->isSelfMessage())
        return;

    if (msg == m_Timer_Report){

        if (param_PeriodOutput){

            std::string command = "mkdir \"" + mRootDirectory +"/" + simTime().str() + "\"";
            system(command.c_str());

            for (int stripe = 0; stripe < m_Stripes; stripe++)
                doReportForStripe(stripe);
        }

        scheduleAt(simTime() + param_PeriodOutputIntervall, m_Timer_Report);
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
        filename << mRootDirectory << "/" << simTime().str() << "/stripe." << stripe << ".sources";
        m_Writer.open(filename.str().c_str());
        if (m_Src != NULL)
            m_Writer << m_Src->getAddress().str() << " " << endl;
        m_Writer.flush(); m_Writer.close();
    //peers
        filename.str("");
        filename << mRootDirectory << "/" << simTime().str() << "/stripe." << stripe << ".parents";
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

    /*if (mPeerjoinTime.find( peer ) != mPeerjoinTime.end()){
        m_PeerOutput << simTime() << " [ERROR] JOIN TIME UNKNOWN " << endl;
        simtime_t stayDuration = (simTime() - mPeerjoinTime.find(peer)->second);

        m_PeerReceiveRate << peer->getAddress().str() << "\t" << peer->getChunksReceived() / stayDuration << endl;
    }*/


    m_Peers.removeItem(peer);

    mStats_Player_Hits  += peer->getPlayer()->getChunksHit();
    mStats_Player_Miss  += peer->getPlayer()->getChunksMissed();
    mStats_Player_Stall += peer->getPlayer()->getChunksStalled();
    mStats_Player_Skip  += peer->getPlayer()->getChunksSkipped();

    mStats_Peers_Leaved++;
    mStats_Peers_Current--;

    m_ChunkMissHits << peer->getAddress().str() << "\t" << peer->getPlayer()->getChunksMissed() << "\t" << peer->getPlayer()->getChunksHit() << endl;
    double tmp = peer->getPlayer()->getChunksMissed() + peer->getPlayer()->getChunksHit();
    m_List_MissRatio.push_back( (double)peer->getPlayer()->getChunksMissed() * 100 / tmp );
}

void MTreeBoneStats::peerStartedPlayer(MTreeBonePeer* peer){
    m_PeerOutput << simTime() << " [PEER] started player: " << peer->getAddress();
    if (mPeerjoinTime.find( peer ) == mPeerjoinTime.end()){
        m_PeerOutput << simTime() << " [ERROR] JOIN TIME UNKNOWN " << endl;
        return;
    }
    double delay = (simTime() - mPeerjoinTime.find(peer)->second).dbl();

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
    m_StartUpDelay << peer->getAddress().str() << "\t" << delay << endl;
    m_List_StartUpDelay.push_back(delay);
}

void MTreeBoneStats::chunkGenerated(SEQUENCE_NUMBER_T chunknumber){
    mChunkStats.insert(std::pair<SEQUENCE_NUMBER_T, cChunkStats*>(chunknumber, new cChunkStats( simTime(), mStats_Peers_Current )) );
}

void MTreeBoneStats::chunkReceived(MTreeBonePeer* peer, SEQUENCE_NUMBER_T chunknumber, int hopcount, bool viaPush){
    //if (mChunkStats.find(chunknumber) == NULL) return;
    if (mChunkStats.find( chunknumber ) == mChunkStats.end()) return;
    cChunkStats* stats = mChunkStats.find(chunknumber)->second;
    if (stats == NULL) return;

    double delay = (simTime() - stats->generated).dbl();

    if ( (delay < stats->minDelay) || (stats->minDelay < 0) )
        stats->minDelay = delay;
    if ( (delay > stats->maxDelay) || (stats->maxDelay < 0) )
        stats->maxDelay = delay;
    stats->totalDelay += delay;
    stats->received++;

    //m_PeerOutput << "[" << chunknumber << "] chunk received, hops: " << hopcount << " push? "<< viaPush << " @ " << peer->getAddress().str() << endl;

    if (viaPush){
        if ( (hopcount < stats->minHopsPush) || (stats->minHopsPush < 0) )
            stats->minHopsPush = hopcount;
        if ( (delay > stats->maxHopsPush) || (stats->maxHopsPush < 0) )
            stats->maxHopsPush = hopcount;
        stats->totalHopsPush += hopcount;
        stats->countHopsPush++;
    }else{
        if ( (hopcount < stats->minHopsPull) || (stats->minHopsPull < 0) )
            stats->minHopsPull = hopcount;
        if ( (delay > stats->maxHopsPull) || (stats->maxHopsPull < 0) )
            stats->maxHopsPull = hopcount;
        stats->totalHopsPull += hopcount;
        stats->countHopsPull++;
    }

    if ((mPeerjoinTime.find( peer ) != mPeerjoinTime.end()) && (mPeerjoinTime.find(peer)->second < stats->generated)){
        if ( (delay < stats->minDelayB) || (stats->minDelayB < 0) )
            stats->minDelayB = delay;
        if ( (delay > stats->maxDelayB) || (stats->maxDelayB < 0) )
            stats->maxDelayB = delay;
        stats->totalDelayB += delay;
        stats->receivedB++;
    }
}

void MTreeBoneStats::chunkPlayed(MTreeBonePeer* peer, SEQUENCE_NUMBER_T chunknumber){
    if (mChunkStats.find( chunknumber ) == mChunkStats.end()) return;
    cChunkStats* stats = mChunkStats.find(chunknumber)->second;
    if (stats == NULL) return;

    double delay = (simTime() - stats->generated).dbl();

    if ( (delay < stats->minPlayed) || (stats->minPlayed < 0) )
        stats->minPlayed = delay;
    if ( (delay > stats->maxPlayed) || (stats->maxPlayed < 0) )
        stats->maxPlayed = delay;
    stats->totalPlayed += delay;
    stats->played++;

    if (peer->getPlayer()->getChunksHit() == 10){
        m_PlayBackDelay << peer->getAddress().str() << "\t" << delay << endl;
        m_List_PlayBackDelay.push_back(delay);
    }
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


    if (mStats_PlayDelay_Count > 0){
        m_PeerOutput << " [STATS][PLAYBACK_DELAY][TOTAL]";
        m_PeerOutput << " Avg: " << (mStats_PlayDelay_SumAverage/mStats_PlayDelay_Count);
        m_PeerOutput << " Observed chunks: " << mStats_PlayDelay_Count;
        m_PeerOutput << endl;
    }

    if (mStats_Hops_Push_Count > 0){
        m_PeerOutput << " [STATS][HOPS][TOTAL][PUSH]";
        m_PeerOutput << " Avg: " << (mStats_Hops_Push_Total/mStats_Hops_Push_Count);
        m_PeerOutput << " Observed chunks: " << mStats_Hops_Push_Count;
        m_PeerOutput << endl;
    }

    if (mStats_Hops_Pull_Count > 0){
        m_PeerOutput << " [STATS][HOPS][TOTAL][PULL]";
        m_PeerOutput << " Avg: " << (mStats_Hops_Pull_Total/mStats_Hops_Pull_Count);
        m_PeerOutput << " Observed chunks: " << mStats_Hops_Pull_Count;
        m_PeerOutput << endl;
    }

    m_PeerOutput << " [STATS][DUPLICATE][TOTAL][PUSH]" << mStats_Duplicate_Push << endl;
    m_PeerOutput << " [STATS][DUPLICATE][TOTAL][PULL]" << mStats_Duplicate_Pull << endl;

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
    m_PeerOutput << " MinB: " << mStats_Last->minDelayB;
    m_PeerOutput << " MaxB: " << mStats_Last->maxDelayB;
    m_PeerOutput << " AvgB: " << (mStats_Last->totalDelayB / mStats_Last->receivedB);
    m_PeerOutput << " ReceivedB: " << mStats_Last->receivedB;
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
    m_PeerOutput << " [STATS][PLAYER][TOTAL]";
    m_PeerOutput << " Hits: " << (mStats_Player_Hits+player_hits);
    m_PeerOutput << " Misses: " << (mStats_Player_Miss+player_misses);
    m_PeerOutput << " Stalled: " << (mStats_Player_Stall+player_stalls);
    m_PeerOutput << " Skipped: " << (mStats_Player_Skip+player_skipped);
    m_PeerOutput << endl;


    m_PeerOutput << " [STATS][PACKETS][TOTAL]";
    long temp = 0;
    for (int i = 0; i < MTREEBONE_PACKETS_MAX+1; i++){
        temp += mStats_Packets[i];
        m_PeerOutput << " type " << i << ": " << mStats_Packets[i];
    }
    m_PeerOutput << " total: " << temp << endl;



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

    m_PeerOutput << " MinB: " << stats->minDelayB;
    m_PeerOutput << " MaxB: " << stats->maxDelayB;
    m_PeerOutput << " AvgB: " << (stats->totalDelayB / stats->receivedB);
    m_PeerOutput << " ReceivedB: " << stats->receivedB;

    m_PeerOutput << " MinPlayed: " << stats->minPlayed;
    m_PeerOutput << " MaxPlayed: " << stats->maxPlayed;
    m_PeerOutput << " AvgPlayed: " << (stats->totalPlayed / stats->played);
    m_PeerOutput << " Played: " << stats->played;

    m_PeerOutput << " Duplicate via Push: " << stats->duplicatesPush;
    m_PeerOutput << " Duplicate via Pull: " << stats->duplicatesPull;

    if (stats->countHopsPush > 0)
        m_PeerOutput << " Hops via Push: Min: " << stats->minHopsPush << " Max: " << stats->maxHopsPush << " Average: " << ((double)stats->totalHopsPush / (double)stats->countHopsPush);
    if (stats->countHopsPull > 0)
        m_PeerOutput << " Hops via Pull: Min: " << stats->minHopsPull << " Max: " << stats->maxHopsPull << " Average: " << ((double)stats->totalHopsPull / (double)stats->countHopsPull);

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

            if ( ((*it).second->minDelayB < mStats_Last->minDelayB) || (mStats_Last->minDelayB < 0) )
                mStats_Last->minDelayB = (*it).second->minDelayB;
            if ( ((*it).second->maxDelayB > mStats_Last->maxDelayB) || (mStats_Last->maxDelayB < 0) )
                mStats_Last->maxDelayB = (*it).second->maxDelayB;
            mStats_Last->totalDelayB += (*it).second->totalDelayB;
            mStats_Last->receivedB   += (*it).second->receivedB;


            mStats_Duplicate_Push += (*it).second->duplicatesPush;
            mStats_Duplicate_Pull += (*it).second->duplicatesPull;

            if ((*it).second->played > 0){
                mStats_PlayDelay_Count++;
                mStats_PlayDelay_SumAverage += (*it).second->totalPlayed / (*it).second->played;
            }

            if ((*it).second->countHopsPush > 0){
                mStats_Hops_Push_Count++;
                mStats_Hops_Push_Total +=  (*it).second->totalHopsPush / (*it).second->countHopsPush;
            }
            if ((*it).second->countHopsPull > 0){
                mStats_Hops_Pull_Count++;
                mStats_Hops_Pull_Total +=  (*it).second->totalHopsPull / (*it).second->countHopsPull;
            }

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

void MTreeBoneStats::registerPacketSend(MTreeBonePacketType type){
    mStats_Packets[type]++;
}

void MTreeBoneStats::onPlayerSkipped(MTreeBonePeer* peer, SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition){
    m_PeerOutput << simTime() << " [DEBUG][PLAYER] skipped from " << oldposition << " to " << newposition << " @ " << peer->getAddress().str() << " bonenode: " << !peer->getParent(0).isUnspecified() << endl;
}

void MTreeBoneStats::chunkDuplicateReceived(MTreeBonePeer* peer, IPvXAddress sender, SEQUENCE_NUMBER_T chunknumber, bool viaPush){
    if (mChunkStats.find( chunknumber ) == mChunkStats.end()) return;
    cChunkStats* stats = mChunkStats.find(chunknumber)->second;
    if (stats == NULL) return;

    if (viaPush)
        stats->duplicatesPush++;
    else
        stats->duplicatesPull++;
}
