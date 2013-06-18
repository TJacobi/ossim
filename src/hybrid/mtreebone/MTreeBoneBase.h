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
#ifndef MTREEBONEBASE_H_
#define MTREEBONEBASE_H_

#include "CommBase.h"
#include "IPvXAddress.h"
#include "GossipProtocolWithUserData.h"
#include "MTreeBonePacket_m.h"
#include "DpControlInfo_m.h"
#include "VideoBuffer.h"
#include "Forwarder.h"
#include "MTreeBoneStripeInformation.h"
#include "MTreeBonePeerInformation.h"
#include "VideoBufferListener.h"

class MTreeBoneBase : public CommBase, public VideoBufferListener {
public:
    MTreeBoneBase();
    virtual ~MTreeBoneBase();

    void handleMessage(cMessage *msg);

    virtual void onNewChunk(IPvXAddress src, int sequenceNumber);
protected:
    GossipProtocolWithUserData* m_Gossiper;

    // parameters
    // @brief maximum number of partners
    unsigned int param_maxNOP;
    // @brief desired number of partners
    unsigned int param_desiredNOP;
    // @brief number of stripes to use
    unsigned int param_numStripes;

    // timer
    cMessage* timer_checkPartners;

    void initBase();

    virtual void processPacket(cPacket *pkt);
    virtual void handleTimerMessage(cMessage *msg);

    void processNeighborRequest(IPvXAddress src, MTreeBoneNeighborRequestPacket* pkt);
    void processNeighborRequestResponse(IPvXAddress src, MTreeBoneNeighborRequestResponsePacket* pkt);
protected:
    int m_localPort, m_destPort;

    // stripe/peer management
    MTreeBoneStripeInformation* m_Stripes;
    std::map<IPvXAddress, MTreeBonePeerInformation*> m_Neighbors;

    MTreeBonePeerInformation* getPeerInformation(IPvXAddress addr);
    void addNeighbor(IPvXAddress addr, int stripe);
    void removeNeighbor(IPvXAddress addr, int stripe);

    virtual MTreeBoneBufferMapPacket* prepareBufferMap();

    // upload management
    // @brief parameter maximum upload factor (x*incoming stream)
    double param_MaxUploadFactor;
    // @brief next reset for upload chunk counter
    SimTime m_UploadNextReset;
    // @brief chunks needed for 1 second of stream
    double m_ChunksPerSecond;
    // @brief chunks left for this window
    double m_ChunksLeftForWindow;
    genericList<int> m_FreeUploadList;

    unsigned int m_ChunksUploaded;
    unsigned int m_ChunksDenied;
    unsigned int m_DebugOutput;

    void handleChunkRequest(IPvXAddress src, MTreeBoneChunkRequestPacket* pkt);

    // timers
    cMessage* timer_sendBufferMaps;

    // -- Pointers to /local/ modules
    VideoBuffer *m_videoBuffer;
    Forwarder   *m_forwarder;
};

#endif /* MTREEBONEBASE_H_ */
