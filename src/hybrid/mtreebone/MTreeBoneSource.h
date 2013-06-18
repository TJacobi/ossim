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

#ifndef MTREEBONESOURCE_H_
#define MTREEBONESOURCE_H_

#include "MTreeBoneBase.h"

class MTreeBoneSource: public MTreeBoneBase {
public:
    MTreeBoneSource();
    virtual ~MTreeBoneSource();

protected:

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void handleTimerMessage(cMessage *msg);

    // @brief override of the base function because the source is bonenode of every stripe
    virtual MTreeBoneBufferMapPacket* prepareBufferMap();
private:

};

#endif /* MTREEBONESOURCE_H_ */
