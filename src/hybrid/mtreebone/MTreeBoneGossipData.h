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

#ifndef MTREEBONEGOSSIPDATA_H_
#define MTREEBONEGOSSIPDATA_H_


//#define DODEBUG
#ifdef DODEBUG
    #include "MyDebugClass.h"
#endif

#include "GossipUserData.h"

class MTreeBoneGossipData: public GossipUserData {
public:
    MTreeBoneGossipData(int numStripes);
    virtual ~MTreeBoneGossipData();

    virtual GossipUserData* dup() const;

    virtual long getSizeInBits();

    void setHeadChunk(unsigned int number){m_HeadChunk = number;}
    unsigned int getHeadChunk(){return m_HeadChunk;}

    void setIsBoneNode(int stripe, bool val){ if (stripe >= m_NumStripes) return; m_IsBoneNode[stripe] = val;}
    bool getIsBoneNode(int stripe){ if (stripe >= m_NumStripes) return false; return m_IsBoneNode[stripe];}

    void setNumChildren(int stripe, int val){ if (stripe >= m_NumStripes) return; m_NumChildren[stripe] = val;}
    int getNumChildren(int stripe){ if (stripe >= m_NumStripes) return 0; return m_NumChildren[stripe];}

    void setDistance(int value){m_Distance = value;}
    int  getDistance(){return m_Distance;}
private:
    int          m_NumStripes;
    bool*        m_IsBoneNode;
    int*         m_NumChildren;
    unsigned int m_HeadChunk; // used as a "distance"-metric
    int          m_Distance;
};

#endif /* MTREEBONEGOSSIPDATA_H_ */
