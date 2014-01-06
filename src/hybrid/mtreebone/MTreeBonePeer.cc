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

#include "MTreeBonePeer.h"
#include "MTreeBoneStats.h"

Define_Module(MTreeBonePeer);

MTreeBonePeer::MTreeBonePeer() {
    // TODO Auto-generated constructor stub

}

MTreeBonePeer::~MTreeBonePeer() {
    // TODO Auto-generated destructor stub
}

void MTreeBonePeer::initialize(int stage){
    if (stage != 3)
        return;
    initBase();

    m_Player = check_and_cast<PlayerBufferSkip*>(getParentModule()->getModuleByRelativePath("player"));
    m_Player->addListener(this);

    m_videoBuffer->addListener(this);
}

void MTreeBonePeer::onPlayerStarted(){
    MTreeBoneStats::theStats->peerStartedPlayer( this );
}
void MTreeBonePeer::onChunksSkipped(SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition){
    MTreeBoneStats::theStats->onPlayerSkipped( this, oldposition, newposition );
}
void MTreeBonePeer::onChunkHit(SEQUENCE_NUMBER_T hit){
    MTreeBoneStats::theStats->chunkPlayed( this, hit);
}
void MTreeBonePeer::onChunkMiss(SEQUENCE_NUMBER_T miss){

}

void MTreeBonePeer::onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    int stripe = sequenceNumber % MTreeBoneSettings::theSettings->getNumberOfStripes();

    if (debugOutput)
        m_outFileDebug << "[DOWN] received chunk " << sequenceNumber << " from " << src.str() << endl;

    MTreeBoneStats::theStats->chunkReceived( this, sequenceNumber, hopcount, m_Stripes[stripe].Parent.equals(src));
}
void MTreeBonePeer::onDuplicateChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    int stripe = sequenceNumber % MTreeBoneSettings::theSettings->getNumberOfStripes();

    MTreeBoneStats::theStats->chunkDuplicateReceived( this, src, sequenceNumber, m_Stripes[stripe].Parent.equals(src));
}

void MTreeBonePeer::onJoinedNetwork(){
    MTreeBoneStats::theStats->peerJoinedNetwork(this);
}
void MTreeBonePeer::onLeavedNetwork(){
    MTreeBoneStats::theStats->peerLeavedNetwork(this);
}
