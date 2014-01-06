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

#include "CommBase.h"
#include "GossipListener.h"
#include "MTreeBoneBase.h"
#include "PlayerListener.h"

class MTreeBoneBase;

class MTreeBonePeer : public MTreeBoneBase, public PlayerListener, public VideoBufferListener {
public:
    MTreeBonePeer();
    virtual ~MTreeBonePeer();

    virtual PlayerBufferSkip*   getPlayer()         {return m_Player;}

    // PlayerListener
    virtual void onPlayerStarted();
    virtual void onChunksSkipped(SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition);
    virtual void onChunkHit(SEQUENCE_NUMBER_T hit);
    virtual void onChunkMiss(SEQUENCE_NUMBER_T miss);

    // VideoBufferListener
    virtual void onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount);
    virtual void onDuplicateChunk(IPvXAddress src, int sequenceNumber, int hopcount);
protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);

    virtual void onJoinedNetwork();
    virtual void onLeavedNetwork();

    PlayerBufferSkip* m_Player;
};

#endif /* MTREEBONEPEER_H_ */
