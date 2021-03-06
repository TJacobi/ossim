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
#include "MTreeBoneStats.h"

Define_Module(MTreeBoneSource);

MTreeBoneSource::MTreeBoneSource() {
    // TODO Auto-generated constructor stub

}

MTreeBoneSource::~MTreeBoneSource() {
    // TODO Auto-generated destructor stub
}

void MTreeBoneSource::initialize(int stage){
    if (stage != 3)
        return;
    initBase();

    m_videoBuffer->addListener(this);

    MTreeBoneStats::theStats->setSource(this);
}

void MTreeBoneSource::onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    MTreeBoneStats::theStats->chunkGenerated(sequenceNumber);
}
