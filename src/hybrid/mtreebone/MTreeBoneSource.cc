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

#include "MTreeBoneSource.h"

Define_Module(MTreeBoneSource);

#include "MTreeBoneStats.h"

MTreeBoneSource::MTreeBoneSource(){
    timer_joinNetwork = NULL;
}

MTreeBoneSource::~MTreeBoneSource() {
    if (timer_joinNetwork)
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;
}

void MTreeBoneSource::initialize(int stage){
    if (stage != 3)
        return;
    initBase();

    MTreeBoneStats::theStats->setSource(this);

    timer_joinNetwork    = new cMessage("MTreeBoneSource_JOIN_NETWORK");
    scheduleAt(SimTime(), timer_joinNetwork);

    // create messages


    // schedule timers

}

#include "NewscastCacheEntry.h"

void MTreeBoneSource::handleTimerMessage(cMessage *msg){

    if (msg == timer_joinNetwork)
    {
        m_Gossiper->joinNetwork();
        cancelAndDelete(timer_joinNetwork); timer_joinNetwork = NULL;
    }
    else
    {
#ifdef MTreeBoneGossipData_DoCounting
        EV << " gossip count: " << MTreeBoneGossipData::count << endl;
#endif
#ifdef NewscastCacheEntry_DoCounting
        EV << " newscast entry count: " << NewscastCacheEntry::count << endl;
#endif
        MTreeBoneBase::handleTimerMessage(msg);
    }
}

bool MTreeBoneSource::isBoneNodeForStripe(int stripe){
    return true;
}

int MTreeBoneSource::getMyDistance(int stripe){
    return 0;
}

void MTreeBoneSource::onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    MTreeBoneStats::theStats->chunkGenerated(sequenceNumber);
    MTreeBoneBase::onNewChunk(src, sequenceNumber, hopcount);
}
