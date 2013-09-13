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

#include "MTreeBonePeerInformation.h"

inline int min(const int a, const int b) { return (a < b) ? a : b; }

MTreeBonePeerInformation::MTreeBonePeerInformation(int stripes, int buffersize) {

    nextRequestTime = 0;

    this->buffersize = buffersize;
    buffermap        = new bool[buffersize];
    this->stripes    = stripes;
    isbonenode       = new bool[stripes];

    distance         = new int[stripes];
    numChildren      = new int[stripes];

    for (int i = 0; i < buffersize; i++)
        buffermap[i] = false;
    for (int i = 0; i < stripes; i++){
        isbonenode[i] = false;
        distance[i]   = -1;
        numChildren[i] = -1;
    }

    sequenceNumberStart = sequenceNumberEnd = 0;

    requestsSend = chunksReceived = 0;

    lastData = NULL;
    desertedPeer = simTime();
    missingChunks = 1000;
}

MTreeBonePeerInformation::~MTreeBonePeerInformation() {
    // TODO Auto-generated destructor stub
    delete []buffermap;
    delete []isbonenode;
    delete []distance;
    delete []numChildren;
}

void MTreeBonePeerInformation::updateFromBufferMap(MTreeBoneBufferMapPacket* pkt){

    if ( (sequenceNumberEnd != pkt->getSequenceNumberEnd()) || (missingChunks > pkt->getMissingChunks()) )
        desertedPeer = simTime();

    sequenceNumberStart = pkt->getSequenceNumberStart();
    sequenceNumberEnd   = pkt->getSequenceNumberEnd();

    EV << "MTreeBonePeerInformation::updateFromBufferMap " << sequenceNumberStart << " , " << sequenceNumberEnd << endl << endl;

    for (int i = 0; i < min(buffersize, pkt->getBufferMapArraySize()); i++){
        buffermap[i] = pkt->getBufferMap(i);
    }

    for (int i = 0; i < min(stripes, pkt->getBoneNodeForStripeArraySize()); i++){
        isbonenode[i] = pkt->getBoneNodeForStripe(i);
    }

    missingChunks = pkt->getMissingChunks();
}

bool MTreeBonePeerInformation::inBuffer(int sequenceNumber){
    if ((sequenceNumber < getSequenceNumberStart()) || (sequenceNumber > getSequenceNumberEnd()))
        return false;

    return buffermap[sequenceNumber % buffersize];
}

int MTreeBonePeerInformation::getDistance(int stripe){
    if (distance[stripe] >= 0)
        return distance[stripe];
    if (lastData == NULL)
        return -1;
    return lastData->getDistance(stripe);
}

int MTreeBonePeerInformation::getNumChildren(int stripe){
    if (numChildren[stripe] >= 0)
        return numChildren[stripe];
    if (lastData == NULL)
        return -1;
    return lastData->getNumChildren(stripe);
}
