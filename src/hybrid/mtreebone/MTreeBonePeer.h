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

class MTreeBonePeer: public MTreeBoneBase {
public:
    MTreeBonePeer();
    virtual ~MTreeBonePeer();

    virtual void onNewChunk(IPvXAddress src, int sequenceNumber);
protected:
protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void handleTimerMessage(cMessage *msg);
    virtual void processPacket(cPacket *pkt);
private:
    // timer
    cMessage* timer_joinNetwork;
    cMessage* timer_checkNeighbors;

    // chunk request handling
    std::map<int, SimTime> m_PendingRequests; // sequence number, timeout

    void checkNeighbors();
    void checkParents();
    void requestNextChunks(IPvXAddress peer, int max = 5);
    bool requestChunkFromPeer(IPvXAddress peer, int sequenceNumber);
    void requestChunk(int sequenceNumber, IPvXAddress notHim = IPvXAddress("0.0.0.0"));

    bool wantToBeBoneNode(int stripe){return true;};

    void handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp);
};

#endif /* MTREEBONEPEER_H_ */
