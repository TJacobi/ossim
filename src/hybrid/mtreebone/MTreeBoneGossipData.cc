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

#include "MTreeBoneGossipData.h"

#ifdef MTreeBoneGossipData_DoCounting
int MTreeBoneGossipData::count = 0;
#endif

MTreeBoneGossipData::MTreeBoneGossipData(int numStripes) {
    m_NumStripes = numStripes;

    m_IsBoneNode  = new bool[numStripes];
    m_NumChildren = new int[numStripes];
    m_Distance    = new int[numStripes];

#ifdef MTreeBoneGossipData_DoCounting
    MTreeBoneGossipData::count++;
#endif
}

MTreeBoneGossipData::~MTreeBoneGossipData() {
#ifdef MTreeBoneGossipData_DoCounting
    MTreeBoneGossipData::count--;
#endif
    delete []m_IsBoneNode;
    delete []m_NumChildren;
    delete []m_Distance;
}

GossipUserData*  MTreeBoneGossipData::dup()const
{
    MTreeBoneGossipData* ret = new MTreeBoneGossipData(m_NumStripes);
    for (int i = 0; i < m_NumStripes; i++){
        ret->setIsBoneNode (i, m_IsBoneNode[i]);
        ret->setNumChildren(i, m_NumChildren[i]);
        ret->setDistance(i, m_Distance[i]);
    }

    return ret;
}

long MTreeBoneGossipData::getSizeInBits()
{
    return 0;
}
