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

#ifndef MTREEBONESIMPLEATTACKER_H_
#define MTREEBONESIMPLEATTACKER_H_

#include "MTreeBoneBase.h"

class MTreeBoneSimpleAttacker : public MTreeBoneBase {
public:
    MTreeBoneSimpleAttacker();
    virtual ~MTreeBoneSimpleAttacker();

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);

    virtual int getReportedNumChildren(int stripe){return m_Stripes[stripe].Children.size() + param_FakeChildren;}

    virtual bool wantToBeBoneNode(int stripe);
    virtual bool sendChunk(SEQUENCE_NUMBER_T chunk, IPvXAddress dest, bool isPush = false);

    bool    isAttacking()   {return m_Attacking;}
protected:
    virtual void onJoinedNetwork();
    virtual void onLeavedNetwork();
    virtual void onTimerCheckNeighbors();

    void checkAttackState();
private:
    bool    param_DenyPush;
    bool    param_DenyPull;
    bool    param_FastUpdateGossiping;
    int     param_FakeChildren;
    int     param_MaxAttackDistance;
    double  param_StartAttackTime;

    bool    m_Attacking;
};

#endif /* MTREEBONESIMPLEATTACKER_H_ */
