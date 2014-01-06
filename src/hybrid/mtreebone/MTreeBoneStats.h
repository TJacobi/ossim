#ifndef MTREEBONESTATS_H_
#define MTREEBONESTATS_H_

#include "genericList.h"
#include "csimplemodule.h"

#include "MTreeBoneSource.h"
#include "MTreeBonePeer.h"

#include "AppSettingDonet.h"
#include <fstream>

class MTreeBoneStats : public cSimpleModule {
public:
    MTreeBoneStats();
    virtual ~MTreeBoneStats();

    void setSource(MTreeBoneSource* src){m_Src = src;}

    void peerJoinedNetwork(MTreeBonePeer* peer);
    void peerLeavedNetwork(MTreeBonePeer* peer);
    void peerStartedPlayer(MTreeBonePeer* peer);

    void chunkGenerated(SEQUENCE_NUMBER_T chunknumber);
    void chunkReceived(MTreeBonePeer* peer, SEQUENCE_NUMBER_T chunknumber, int hopcount, bool viaPush);
    void chunkPlayed(MTreeBonePeer* peer, SEQUENCE_NUMBER_T chunknumber);

    void chunkSendViaPush(IPvXAddress src, IPvXAddress dst, SEQUENCE_NUMBER_T chunknumber);
    void chunkSendViaPull(IPvXAddress src, IPvXAddress dst, SEQUENCE_NUMBER_T chunknumber);

    void chunkDuplicateReceived(MTreeBonePeer* peer, IPvXAddress sender, SEQUENCE_NUMBER_T chunknumber, bool viaPush);

    void debugChunksSkipped(IPvXAddress addr, SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition){
        m_PeerOutput << simTime() << " [PLAYER][SKIP] player " << addr.str() << " skipped from " << oldposition << " to " << newposition << " count: " << (newposition-oldposition) << endl;
    }

    void registerPacketSend(MTreeBonePacketType type);

    void onPlayerSkipped(MTreeBonePeer* peer, SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition);

    // attacker
    void attackerJoinedNetwork(MTreeBoneBase* attacker);
    void attackerLeavedNetwork(MTreeBoneBase* attacker);


    static MTreeBoneStats* theStats; // global pointer for easier acces

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();
    void handleMessage(cMessage *msg);
protected:

    class cChunkStats{
    public:
        cChunkStats (simtime_t gen, int peers){
            generated = gen;
            received = receivedB = 0;
            minDelay = maxDelay = minDelayB = maxDelayB = -1;
            totalDelay = totalDelayB = 0;
            peersAtGenerations = peers;

            played = 0;
            minPlayed = maxPlayed = -1;
            totalPlayed = 0;
            duplicatesPush = duplicatesPull = 0;

            minHopsPush = maxHopsPush = minHopsPull = maxHopsPull = -1;
            totalHopsPush = countHopsPush = totalHopsPull = countHopsPull = 0;
        }
        simtime_t generated;
        int received, peersAtGenerations, receivedB;
        double minDelay, maxDelay, totalDelay;
        double minDelayB, maxDelayB, totalDelayB;

        int played;
        double minPlayed, maxPlayed, totalPlayed;

        long duplicatesPush, duplicatesPull;

        int minHopsPush, maxHopsPush, totalHopsPush, countHopsPush;
        int minHopsPull, maxHopsPull, totalHopsPull, countHopsPull;

    };

    class cPlayerStats{
    public:
        cPlayerStats (long hits, long miss){
            mHits = hits;
            mMiss = miss;
        }

        long mHits, mMiss;
    };

    MTreeBoneSource* m_Src;
    genericList<MTreeBonePeer*> m_Peers;
    genericList<MTreeBoneBase*> m_Attackers;
    int m_Stripes;

    std::ofstream m_PeerOutput;

    std::ofstream m_StartUpDelay;
    std::list<double> m_List_StartUpDelay;
    std::ofstream m_PlayBackDelay;
    std::list<double> m_List_PlayBackDelay;
    std::ofstream m_ChunkMissHits;
    std::list<double> m_List_MissRatio;

    std::ofstream m_PeerReceiveRate;
    std::ofstream m_ContinuityAvg;
    std::ofstream m_AttackedChildrenFile;

    cMessage* m_Timer_Report;
    cMessage* m_Timer_Report_Stats;
    cMessage* m_Timer_Continuity;
    cMessage* m_Timer_Attacked_Children;

    std::map<MTreeBonePeer*, simtime_t> mPeerjoinTime;
    std::map<SEQUENCE_NUMBER_T, cChunkStats*> mChunkStats;
    std::map<MTreeBonePeer*, cPlayerStats*> mContinuityIndexData;

    void doReportForStripe(int stripe);
    void doPrintContinuityIndex();

    double mStats_StartUpDelay_Min, mStats_StartUpDelay_Max, mStats_StartUpDelay_Total;
    int mStats_StartUpDelay_Count;

    double mStats_StartUpDelay_Min_t, mStats_StartUpDelay_Max_t, mStats_StartUpDelay_Total_t;
    int mStats_StartUpDelay_Count_t;

    int mStats_Peers_Joined, mStats_Peers_Leaved, mStats_Peers_Current, mStats_Peers_MaxConcurrent;
    int mStats_Peers_Joined_t, mStats_Peers_Leaved_t, mStats_Peers_Current_t, mStats_Peers_MaxConcurrent_t;

    int mStats_Attackers_TotalJoined;
    cChunkStats* mStats_Last;

    // Playback delay
    int mStats_PlayDelay_Count;
    double mStats_PlayDelay_SumAverage;

    long mStats_Chunks_Pushed, mStats_Chunks_Pulled, mStats_Chunks_Pushed_t, mStats_Chunks_Pulled_t;

    long* mStats_Packets;

    long mStats_Player_Hits, mStats_Player_Miss, mStats_Player_Stall, mStats_Player_Skip;

    long mStats_Hops_Push_Count, mStats_Hops_Pull_Count;
    double mStats_Hops_Push_Total, mStats_Hops_Pull_Total;

    long mStats_Duplicate_Push,mStats_Duplicate_Pull;

    void cleanOldData(simtime_t thresh = 60);

    void printStats();
    void printChunkStats(SEQUENCE_NUMBER_T chunknumber);

    bool param_PeriodOutput;
    double param_PeriodOutputIntervall;
    double param_PeriodContinuityIntervall;
    double param_AttackedChildrenIntervall;



    MTreeBoneBase* getPeer(IPvXAddress addr, bool searchAttackers = false);
    long getChildrenCount(MTreeBoneBase* peer, bool recursive, bool ignoreAttackers);

private:
    std::string mRootDirectory;
};

#endif /* MTREEBONESTATS_H_ */
