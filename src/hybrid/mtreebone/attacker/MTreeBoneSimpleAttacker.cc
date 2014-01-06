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

#include "MTreeBoneSimpleAttacker.h"
#include "MTreeBoneStats.h"

Define_Module(MTreeBoneSimpleAttacker);

MTreeBoneSimpleAttacker::MTreeBoneSimpleAttacker() {
    // TODO Auto-generated constructor stub
    param_FakeChildren = 0;
    param_FastUpdateGossiping = false;

    m_Attacking = false;
}

MTreeBoneSimpleAttacker::~MTreeBoneSimpleAttacker() {
    // TODO Auto-generated destructor stub
}

void MTreeBoneSimpleAttacker::initialize(int stage){
    if (stage != 3)
        return;

    param_DenyPull              = par("denyPull").boolValue();
    param_DenyPush              = par("denyPush").boolValue();
    param_FastUpdateGossiping   = par("fastUpdateGossiping").boolValue();
    param_FakeChildren          = par("numFakeChildren");
    param_MaxAttackDistance     = par("maxAttackDistance");
    param_StartAttackTime       = par("startAttackTime").doubleValue();

    initBase();
}

bool MTreeBoneSimpleAttacker::sendChunk(SEQUENCE_NUMBER_T chunk, IPvXAddress dest, bool isPush){

    int stripe = chunk % MTreeBoneSettings::theSettings->getNumberOfStripes();

    // dont attack if we havent reached position yet
    if ((param_MaxAttackDistance > 0) && (param_MaxAttackDistance < getMyDistance(stripe))){
        m_Attacking = false;
        return MTreeBoneBase::sendChunk(chunk, dest, isPush);
    }

    /*
    // dont attack if it is not time yet
    if ( simTime() < param_StartAttackTime )
        return MTreeBoneBase::sendChunk(chunk, dest, isPush);
    */


    if (m_Attacking){
        if (isPush && param_DenyPush) return false;
        if (!isPush && param_DenyPull) return false;
    }

    return MTreeBoneBase::sendChunk(chunk, dest, isPush);
}

void MTreeBoneSimpleAttacker::onJoinedNetwork(){
    MTreeBoneStats::theStats->attackerJoinedNetwork(this);
}
void MTreeBoneSimpleAttacker::onLeavedNetwork(){
    MTreeBoneStats::theStats->attackerLeavedNetwork(this);
}

void MTreeBoneSimpleAttacker::onTimerCheckNeighbors(){
    checkAttackState();
    if (param_FastUpdateGossiping)
        m_Gossiper->requestUpdate();
};

void MTreeBoneSimpleAttacker::checkAttackState(){

    m_Attacking = ( simTime() >= param_StartAttackTime );
    return;
}

bool MTreeBoneSimpleAttacker::wantToBeBoneNode(int stripe){
    return true;
}
