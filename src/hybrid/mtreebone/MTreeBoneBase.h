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

#include "PlayerBufferSkip.h"

#include <fstream>

#ifndef MTREEBONEBASE_H_
#define MTREEBONEBASE_H_

#include "CommBase.h"
#include "GossipProtocolWithUserData.h"
#include "GossipListener.h"
#include "MTreeBoneStripeInformation.h"
#include "MTreeBonePeerInformation.h"
#include "MTreeBoneSettings.h"
#include "VideoBuffer.h"
#include "VideoBufferListener.h"
#include "Forwarder.h"
#include "PlayerListener.h"


class MTreeBoneMeshInterface;
class MTreeBoneTreeInterface;
#include "MTreeBoneInterfaces.h"

class MTreeBoneBase : public CommBase, public GossipListener{
public:
    MTreeBoneBase();
    virtual ~MTreeBoneBase();

    void handleMessage(cMessage *msg);
    virtual void onGossipDataReceived();


    IPvXAddress getAddress(){ return m_localAddress; }

    virtual bool isSource(){return false;}  // overwrite on source

    // upload management
    void setReservedBandwidthFactor(double factor)  {m_ReservedBandwidthFactor = factor;}
    double getReservedBandwidthFactor()             {return m_ReservedBandwidthFactor;}
    double getMaximumBandwidthFactor()              {return param_MaxUploadFactor;}
    double getRemainingBandwidthFactor()            {return param_MaxUploadFactor - m_ReservedBandwidthFactor;}

    virtual bool wantToBeBoneNode(int stripe);
    virtual bool isBoneNodeForStripe(int stripe);
    virtual int getMyDistance(int stripe);

    // sends a chunk to target peer if bandwidth left
    virtual bool sendChunk(SEQUENCE_NUMBER_T chunk, IPvXAddress dest, bool isPush = false);
    void sendPacket(cPacket *pkt, IPvXAddress dest);

    // peer information
    MTreeBoneStripeInformation* getStripe(int stripe){return &(m_Stripes[stripe]);}
    virtual int                 getReportedNumChildren(int stripe){return m_Stripes[stripe].Children.size();}
    MTreeBonePeerInformation*   getPeerInformation(IPvXAddress addr);
    SEQUENCE_NUMBER_T           getHeadSequenceNumber();

    void addNeighbor(IPvXAddress addr, int stripe);
    void removeNeighbor(IPvXAddress addr, int stripe);
    std::map<IPvXAddress, MTreeBonePeerInformation*>* getAllNeighbors(){ return &m_Neighbors; }

    simtime_t getJoinTime(){ return par("joinTime").doubleValue(); };
    bool hasLeftNetwork(){ return m_LeftNetwork; }

    // Modules
    VideoBuffer*                getVideoBuffer()    {return m_videoBuffer;}
    SEQUENCE_NUMBER_T           getOwnHead();
    Forwarder*                  getForwarder()      {return m_forwarder;}
    virtual PlayerBufferSkip*   getPlayer()         {return NULL;}
    SEQUENCE_NUMBER_T           getCurrentPlaybackPoint(void);

    GossipProtocolWithUserData* getGossiper()       {return m_Gossiper;}

    // DEBUG ->
    bool debugOutput;
    std::ofstream m_outFileDebug;
protected:
    virtual void handleTimerMessage(cMessage *msg);

    void initBase();

    virtual void updateOwnGossipData();
    virtual void onJoinedNetwork(){};
    virtual void onLeavedNetwork(){};
    virtual void onTimerCheckNeighbors(){};

    int m_localPort, m_destPort;
    // upload management
    // @brief parameter maximum upload factor (x*incoming stream)
    double param_MaxUploadFactor;
    // @brief next reset for upload chunk counter
    SimTime m_UploadNextReset;
    // @brief chunks needed for 1 second of stream
    double m_ChunksPerSecond;
    // @brief chunks left for this window
    double m_ChunksLeftForWindow;
    double m_FreeChunksLeftForWindow;
    double m_ReservedBandwidthFactor;

    bool m_LeftNetwork;

    // stripe/peer management
    MTreeBoneStripeInformation* m_Stripes;
    std::map<IPvXAddress, MTreeBonePeerInformation*> m_Neighbors;

    // -- Pointers to /local/ modules
    VideoBuffer *m_videoBuffer;
    Forwarder   *m_forwarder;
    GossipProtocolWithUserData* m_Gossiper;
    MTreeBoneMeshInterface* m_Mesh;
    MTreeBoneTreeInterface* m_Tree;

    // Timer
    cMessage* timer_joinNetwork;
    cMessage* timer_leaveNetwork;
    cMessage* timer_checkNeighbors;
    cMessage* timer_chunkScheduler;
    cMessage* timer_sendBufferMaps;

private:
    /*void checkFreeUploadListState();
    bool m_FreeUploadListEnabled;
    genericList<int> m_FreeUploadList;*/
};

#endif /* MTREEBONEBASE_H_ */
