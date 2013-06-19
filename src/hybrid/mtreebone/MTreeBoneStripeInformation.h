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

#ifndef MTREEBONESTRIPE_H_
#define MTREEBONESTRIPE_H_

#include "IPvXAddress.h"
#include "genericList.h"

class MTreeBoneStripeInformation {
public:
    MTreeBoneStripeInformation();
    virtual ~MTreeBoneStripeInformation();

    int HeadChunk;
    int StartChunk;
    int EndChunk;

    genericList<IPvXAddress> Children;
    genericList<IPvXAddress> Neighbors;
    IPvXAddress Parent;
    SimTime nextParentRequest;

    bool isBoneNode(){
        return !Parent.isUnspecified(); // Parent = we are bone node for this stripe
    }
};

#endif /* MTREEBONESTRIPE_H_ */
