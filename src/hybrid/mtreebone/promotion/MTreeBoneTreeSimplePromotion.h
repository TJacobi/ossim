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

#ifndef MTREEBONETREESIMPLEPROMOTION_H_
#define MTREEBONETREESIMPLEPROMOTION_H_

#include "MTreeBoneTree.h"

class MTreeBoneTreeSimplePromotion : public MTreeBoneTree {
public:
    MTreeBoneTreeSimplePromotion();
    virtual ~MTreeBoneTreeSimplePromotion();

    virtual void timerCheckNeighbors();

    virtual void processPacket(MTreeBonePacketTree* pkt, IPvXAddress src);// cPacket *pkt);


    virtual void initialize(int stage);
private:
    bool isPromoted;

    bool isEligibleForPromotion(MTreeBonePeerInformation* info);

    bool param_UseJoinTime;
};

#endif /* MTREEBONETREESIMPLEPROMOTION_H_ */
