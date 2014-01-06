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

#ifndef MTreeBONEMESH_H_
#define MTreeBONEMESH_H_

#include "MTreeBoneInterfaces.h"
#include "MTreeBoneBase.h"

class MTreeBoneMesh : public MTreeBoneMeshInterface{
public:
    MTreeBoneMesh();
    virtual ~MTreeBoneMesh();

    virtual void timerChunkScheduler();
    virtual void timerBufferMaps();
    virtual void timerCheckNeighbors();

    void handleMessage(cMessage *msg);
    virtual void processPacket(cPacket *pkt);
protected:
    MTreeBoneBufferMapPacket* prepareBufferMap();

    void processNeighborRequest(IPvXAddress src, MTreeBoneNeighborRequestPacket* pkt);
    void processNeighborRequestResponse(IPvXAddress src, MTreeBoneNeighborRequestResponsePacket* pkt);
    void handleChunkRequestList(IPvXAddress src, MTreeBoneChunkRequestListPacket* pkt);

private:
    void doChunkSchedule();
    void doChunkSchedule(unsigned int stripe, SEQUENCE_NUMBER_T playerposition);

    int getNumberOfPeersWithChunk(int stripe, int chunk);
    IPvXAddress getFirstPeerWithChunk(int stripe, int chunk);

    std::map<SEQUENCE_NUMBER_T, SimTime> m_PendingRequests; // sequence number, timeout
    bool requestIsPending(SEQUENCE_NUMBER_T sequenceNumber);
    void removePendingRequest(SEQUENCE_NUMBER_T sequenceNumber);
};

#endif /* MTreeBONEMESH_H_ */
