#ifndef MTREEBONESTATS_H_
#define MTREEBONESTATS_H_

#include "genericList.h"
#include "csimplemodule.h"

#include "MTreeBoneSource.h"
#include "MTreeBonePeer.h"
#include "AppSettingDonet.h"

class MTreeBoneStats : public cSimpleModule {
public:
    MTreeBoneStats();
    virtual ~MTreeBoneStats();

    void setSource(MTreeBoneSource* src){m_Src = src;}

    void peerJoinedNetwork(MTreeBonePeer* peer);
    void peerLeavedNetwork(MTreeBonePeer* peer);
    void peerStartedPlayer(MTreeBonePeer* peer);

    void chunkGenerated(SEQUENCE_NUMBER_T chunknumber);
    void chunkReceived(MTreeBonePeer* peer, SEQUENCE_NUMBER_T chunknumber);

    void chunkSendViaPush(IPvXAddress src, IPvXAddress dst, SEQUENCE_NUMBER_T chunknumber);
    void chunkSendViaPull(IPvXAddress src, IPvXAddress dst, SEQUENCE_NUMBER_T chunknumber);

    void debugChunksSkipped(IPvXAddress addr, SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition){
        m_PeerOutput << simTime() << " [PLAYER][SKIP] player " << addr.str() << " skipped from " << oldposition << " to " << newposition << " count: " << (newposition-oldposition) << endl;
    }


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
            received = 0;
            minDelay = maxDelay = -1;
            totalDelay = 0;
            peersAtGenerations = peers;
        }
        simtime_t generated;
        int received, peersAtGenerations;
        simtime_t minDelay, maxDelay, totalDelay;
    };

    MTreeBoneSource* m_Src;
    genericList<MTreeBonePeer*> m_Peers;
    int m_Stripes;

    std::ofstream m_PeerOutput;

    cMessage* m_Timer_Report;
    cMessage* m_Timer_Report_Stats;

    std::map<MTreeBonePeer*, simtime_t> mPeerjoinTime;
    std::map<SEQUENCE_NUMBER_T, cChunkStats*> mChunkStats;

    void doReportForStripe(int stripe);

    simtime_t mStats_StartUpDelay_Min, mStats_StartUpDelay_Max, mStats_StartUpDelay_Total;
    int mStats_StartUpDelay_Count;

    simtime_t mStats_StartUpDelay_Min_t, mStats_StartUpDelay_Max_t, mStats_StartUpDelay_Total_t;
    int mStats_StartUpDelay_Count_t;

    int mStats_Peers_Joined, mStats_Peers_Leaved, mStats_Peers_Current, mStats_Peers_MaxConcurrent;
    int mStats_Peers_Joined_t, mStats_Peers_Leaved_t, mStats_Peers_Current_t, mStats_Peers_MaxConcurrent_t;

    cChunkStats* mStats_Chunks;
    cChunkStats* mStats_Last;

    long mStats_Chunks_Pushed, mStats_Chunks_Pulled, mStats_Chunks_Pushed_t, mStats_Chunks_Pulled_t;

    void cleanOldData(simtime_t thresh = 60);

    void printStats();
    void printChunkStats(SEQUENCE_NUMBER_T chunknumber);
};

#endif /* MTREEBONESTATS_H_ */
