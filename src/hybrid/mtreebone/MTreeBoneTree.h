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

#ifndef MTreeBONETREE_H_
#define MTreeBONETREE_H_

#include "MTreeBoneInterfaces.h"
#include "MTreeBoneBase.h"
#include "VideoBufferListener.h"

class MTreeBoneTree : public MTreeBoneTreeInterface, public VideoBufferListener{
public:
    MTreeBoneTree();
    virtual ~MTreeBoneTree();

    virtual void doInit();
    virtual void leaveNetwork();

    virtual void timerCheckNeighbors();

    void handleMessage(cMessage *msg);
    virtual void processPacket(MTreeBonePacketTree* pkt, IPvXAddress src);// cPacket *pkt);

    virtual void onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount);

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
protected:
    IPvXAddress findBetterParent(int stripe);
    IPvXAddress findSwapCandidate(int stripe);

    void requestParentShip(int stripe, IPvXAddress addr);

    void removeChild(int stripe, IPvXAddress addr);
    void removeParent(int stripe);

    void handleParentRequest(IPvXAddress src, MTreeBoneParentRequestPacket* pkt);
    void handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp);

    void handleReplaceChild(IPvXAddress src, MTreeBonePeerReplaceChildPacket* pkt);

    void handleSwitchPositionRequest(IPvXAddress src, MTreeBonePeerSwitchPostionRequestPacket* resp);
    void handleSwitchPositionResponse(IPvXAddress src, MTreeBonePeerSwitchPostionRequestResponsePacket* resp);

private:
    bool param_DisablePush;

    simtime_t parentTimeout;
};

#endif /* MTreeBONETREE_H_ */
