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

#ifndef MTREEBONEPEERINFORMATION_H_
#define MTREEBONEPEERINFORMATION_H_

#include "MTreeBonePacket_m.h"
#include "MTreeBonePacket_m.h"
#include "MTreeBoneGossipData.h"
#include "AppCommon.h"

class MTreeBonePeerInformation {
public:
    MTreeBonePeerInformation(int stripes, int buffersize);
    virtual ~MTreeBonePeerInformation();

    void updateFromBufferMap(MTreeBoneBufferMapPacket* pkt);

    bool isProbablyDesertedPeer(){return (simTime() - desertedPeer > 10);}

    int getSequenceNumberStart(){return sequenceNumberStart;}
    int getSequenceNumberEnd(){return sequenceNumberEnd;}
    bool inBuffer(int sequenceNumber);

    bool isBoneNode(int stripe){return isbonenode[stripe];}

    SimTime nextRequestTime;

    unsigned int requestsSend;
    unsigned int chunksReceived;

    double getQuality(){
        if (requestsSend == 0) return 1;
        double quality = (double)chunksReceived / (double)requestsSend;
        return (quality < 0.1) ? 0.1 : quality;
    }

    int getDistance(int stripe);
    int getNumChildren(int stripe);

    void setGossipData(MTreeBoneGossipData* data){

        if (lastData != NULL){
            delete lastData;
            lastData = NULL;
        }

        lastData = (data == NULL) ? NULL :check_and_cast<MTreeBoneGossipData*>( data->dup() );

    }

    void setDistance(int stripe, int value){distance[stripe] = value;}
    void setNumChildren(int stripe, int value){numChildren[stripe] = value;}

    void setSequenceNumberEnd(SEQUENCE_NUMBER_T newHead){sequenceNumberEnd = newHead;}

    simtime_t getJoinTime(){return mJoinTime;}
    void setJoinTime(simtime_t time){mJoinTime = time;}

    simtime_t getFirstSeenTime(){return mFirstSeenTime;}
    void setFirstSeenTime(simtime_t time){mFirstSeenTime = time;}
    void setFirstSeenTime(){
        if (mFirstSeenTime <= 0)
            mFirstSeenTime = simTime();
    }
private:
    unsigned int sequenceNumberStart;
    unsigned int sequenceNumberEnd;

    int buffersize;
    bool* buffermap;
    int stripes;
    bool* isbonenode;

    SimTime desertedPeer;
    MTreeBoneGossipData* lastData;
    int* distance;
    int* numChildren;
    int missingChunks;

    simtime_t mJoinTime;
    simtime_t mFirstSeenTime;
};

#endif /* MTREEBONEPEERINFORMATION_H_ */
