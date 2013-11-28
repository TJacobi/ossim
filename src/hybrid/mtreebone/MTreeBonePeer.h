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

#ifndef MTREEBONEPEER_H_
#define MTREEBONEPEER_H_

#include "MTreeBoneBase.h"
#include "PlayerStallSkip.h"
#include "PlayerBufferSkip.h"
#include "PlayerListener.h"

class MTreeBonePeer: public MTreeBoneBase, public PlayerListener {
public:
    MTreeBonePeer();
    virtual ~MTreeBonePeer();

    // VideoBufferListener
    virtual void onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount);
    virtual void onDuplicateChunk(IPvXAddress src, int sequenceNumber, int hopcount);

    // PlayerListener:
    void onPlayerStarted();
    virtual void onChunksSkipped(SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition);
    virtual void onChunkHit(SEQUENCE_NUMBER_T hit);
    virtual void onChunkMiss(SEQUENCE_NUMBER_T miss);

    PlayerBufferSkip* getPlayer(){return mPlayer;}
    //PlayerStallSkip* getPlayer(){return mPlayer;}
protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void handleTimerMessage(cMessage *msg);
    virtual void processPacket(cPacket *pkt);

    void requestParentShip(int stripe, IPvXAddress addr);
    void removeParent(int stripe);
private:
    // timer
    cMessage* timer_joinNetwork;
    cMessage* timer_leaveNetwork;
    cMessage* timer_checkNeighbors;
    cMessage* timer_chunkScheduler;

    PlayerBufferSkip* mPlayer;
    //PlayerStallSkip* mPlayer;
    // chunk request handling
    bool param_DisablePush;
    double param_ChunkScheduleInterval;
    double param_ChunkRequestTimeout;

    std::map<int, SimTime> m_PendingRequests; // sequence number, timeout

    void checkNeighbors();
    void checkParents();

    IPvXAddress findBetterParent(int stripe);
    IPvXAddress findSwapCandidate(int stripe);

    bool wantToBeBoneNode(int stripe); // simple method to disable the usage of push/parents};

    void handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp);

    void handleSwitchPositionRequest(IPvXAddress src, MTreeBonePeerSwitchPostionRequestPacket* resp);
    void handleSwitchPositionResponse(IPvXAddress src, MTreeBonePeerSwitchPostionRequestResponsePacket* resp);

    virtual void doChunkSchedule();
    virtual void doChunkSchedule(unsigned int stripe);

    int getNumberOfPeersWithChunk(int stripe, int chunk);
    IPvXAddress getFirstPeerWithChunk(int stripe, int chunk);

    bool requestIsPending(unsigned int sequenceNumber);
    void removePendingRequest(unsigned int sequenceNumber);

    long m_ChunksReceived;
public:
    long getChunksReceived(){return m_ChunksReceived;}
};

#endif /* MTREEBONEPEER_H_ */
