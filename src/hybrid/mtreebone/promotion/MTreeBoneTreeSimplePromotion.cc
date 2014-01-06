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

#include "MTreeBoneTreeSimplePromotion.h"

Define_Module(MTreeBoneTreeSimplePromotion);

MTreeBoneTreeSimplePromotion::MTreeBoneTreeSimplePromotion() {
    // TODO Auto-generated constructor stub
    isPromoted = false;
}

MTreeBoneTreeSimplePromotion::~MTreeBoneTreeSimplePromotion() {
    // TODO Auto-generated destructor stub
}


void MTreeBoneTreeSimplePromotion::timerCheckNeighbors(){
    if ( (!isPromoted) && (!m_Base->isSource()) )
        return;

    // check neighbors for promotion

    std::map<IPvXAddress, MTreeBonePeerInformation*>::iterator it;
    for (it = m_Base->getAllNeighbors()->begin(); it != m_Base->getAllNeighbors()->end(); it++){
        if (it->second->isBoneNode(0))
            continue;

        if (isEligibleForPromotion(it->second)){
            MTreeBonePromotionPacket* promotion = new MTreeBonePromotionPacket();
            m_Base->sendPacket( promotion, it->first );
        }


    }



    MTreeBoneTree::timerCheckNeighbors();
}

void MTreeBoneTreeSimplePromotion::processPacket(MTreeBonePacketTree* pkt, IPvXAddress src){//cPacket *pkt){
//void MTreeBoneTreeSimplePromotion::processPacket(cPacket *pkt){
    if (m_Base->hasLeftNetwork())
        return;

    switch (check_and_cast<MTreeBonePacketTree*>(pkt)->getPacketType()){
        case MTREEBONE_PROMOTION:
            if (m_Base->debugOutput){
                m_Base->m_outFileDebug << "[DEBUG] received " << MTREEBONE_PROMOTION << endl;
                m_Base->m_outFileDebug << "[PROMOTION] got promoted from: " << src.str() << endl;
            }

            isPromoted = true;
            break;
        default:
            MTreeBoneTree::processPacket(pkt, src);
            break;
    }
}

bool MTreeBoneTreeSimplePromotion::isEligibleForPromotion(MTreeBonePeerInformation* info){

    simtime_t stayedfor = simTime() - info->getJoinTime();
    if (!param_UseJoinTime)
        stayedfor = simTime() - info->getFirstSeenTime();
    simtime_t remaining = remaining.parse(ev.getConfig()->getConfigValue("sim-time-limit")) - simTime();//*2 for testing


    // use default mtreebone algorithm ....
    double kResult = 0.3;
    if (MTreeBoneSettings::theSettings != NULL)
        kResult = MTreeBoneSettings::theSettings->getKResult();

    // test for early promotion -> this test can lead to a node wanting to be a bonenode->connecting->failed or disconnected-> not want to be a bonenode anymore ...
    if ((remaining * kResult - stayedfor.dbl() + 1) > 0){
        double test;
        test = 1 / (remaining * kResult - stayedfor.dbl() + 1);
        if (dblrand() < test) return true;
    }

    return ( stayedfor > remaining * kResult);
}

void MTreeBoneTreeSimplePromotion::initialize(int stage){
    MTreeBoneTree::initialize(stage);

    if (stage != 3)
        return;

    param_UseJoinTime = par("useJoinTime").boolValue();
}
