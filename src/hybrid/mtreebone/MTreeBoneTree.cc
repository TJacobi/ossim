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

#include "MTreeBoneTree.h"
#include "MTreeBoneStats.h"

Define_Module(MTreeBoneTree);

MTreeBoneTree::MTreeBoneTree() {
    // TODO Auto-generated constructor stub

}

MTreeBoneTree::~MTreeBoneTree() {
    // TODO Auto-generated destructor stub
}

void MTreeBoneTree::doInit(){
    m_Base->getVideoBuffer()->addListener(this);
}

void MTreeBoneTree::leaveNetwork(){
    genericList<IPvXAddress>::iterator it;

    // remove parents and children for each stripe
    for (unsigned int stripe = 0; stripe < MTreeBoneSettings::theSettings->getNumberOfStripes(); stripe++){
        MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);
        removeParent(stripe);

        while (pStripe->Children.size() > 0)
            removeChild( stripe, pStripe->Children.at(0) );
        /*it = pStripe->Children.begin();
        while (it != pStripe->Children.end()){
            removeChild( stripe, (*it) );
            it++;
        }*/
        //for (it = pStripe->Children.begin(); it != pStripe->Children.end(); it++)
        //    removeChild(stripe, (*it) );
    }
}

void MTreeBoneTree::onNewChunk(IPvXAddress src, int sequenceNumber, int hopcount){
    // takeover context, because we may send something
    cComponent* origContext = simulation.getContext();
    simulation.setContext(this);

    int stripe = sequenceNumber % MTreeBoneSettings::theSettings->getNumberOfStripes();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);
    std::vector<IPvXAddress>::iterator it;

    //bool count = false;

    if (src.equals(pStripe->Parent))
        parentTimeout = simTime() + 30;

    MTreeBonePeerInformation* info;

    for ( it = pStripe->Children.begin(); it != pStripe->Children.end(); it++){

        info = m_Base->getPeerInformation((*it));
        if ( (info != NULL) && (info->getSequenceNumberEnd() >= sequenceNumber))    // assume peer already has the chunk if his head is before this chunk
            continue;

        //MTreeBoneStats::theStats->chunkSendViaPush(m_localAddress, (*it), sequenceNumber);

        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [UP] forwarding received chunk " << sequenceNumber << " to " << (*it).str() << endl;

        bool result = m_Base->sendChunk(sequenceNumber, *it, true);

        if (m_Base->debugOutput){
            if (result)
                m_Base->m_outFileDebug << simTime().str() << " [UP] forwarded received chunk " << sequenceNumber << " to " << (*it).str() << endl;
            else
                m_Base->m_outFileDebug << simTime().str() << " [UP] could not forward received chunk " << sequenceNumber << " to " << (*it).str() << endl;
        }
        //m_forwarder->sendChunk(sequenceNumber, *it, m_destPort);
        //count = true;
    }

    /*if (count)
        m_FreeChunksLeftForWindow--; //since we are forwarding a "unique" chunk
    else if (m_FreeUploadListEnabled) // add it to the list
        m_FreeUploadList.addItem(sequenceNumber);*/

    // reset context
    simulation.setContext(origContext);
}

void MTreeBoneTree::timerCheckNeighbors(){
    if (m_Base->isSource()) // we dont check if we are source
        return;

    IPvXAddress addr;
    MTreeBonePeerInformation* info;
    MTreeBoneStripeInformation* pStripe;

    double totalChildren = 0;

    for (unsigned int stripe = 0; stripe < MTreeBoneSettings::theSettings->getNumberOfStripes(); stripe ++){
        pStripe = m_Base->getStripe(stripe);

        totalChildren += pStripe->Children.size();

        if (pStripe->nextParentRequest > simTime()) // not waited long enough ...
            continue;

        if (!m_Base->wantToBeBoneNode(stripe)) // we dont want to be a bone node?
            continue;

        if (pStripe->isBoneNode() && (parentTimeout > simTime()) ){

            IPvXAddress SwapCandidate = findSwapCandidate(stripe);

            if (SwapCandidate.isUnspecified()){ // no usefull swap candidate ...
                // look if we can find a better parent ...
                IPvXAddress newParentSuggestion = findBetterParent(stripe);

                if (!newParentSuggestion.isUnspecified())
                    requestParentShip(stripe, newParentSuggestion);
            }else{
                if (m_Base->debugOutput)
                    m_Base->m_outFileDebug << simTime().str() << " [SWITCH] switching position with: " << SwapCandidate.str() << " for stripe: " << stripe << endl;

                MTreeBonePeerSwitchPostionRequestPacket* req = new MTreeBonePeerSwitchPostionRequestPacket();
                req->setStripe(stripe);
                req->setSuggestedNewParent( pStripe->Parent );

                m_Base->sendPacket(req, SwapCandidate);
                //sendToDispatcher(req, m_localPort, SwapCandidate, m_destPort);
            }
        }else{  // not a bone node OR we have not received a chunk from our parent for a while ...
            //if (!pStripe->isBoneNode())
                while (pStripe->Children.size() > 0)
                    removeChild(stripe, pStripe->Children.at(0));

            if (m_Base->debugOutput)
                m_Base->m_outFileDebug << simTime().str() << " [PARENT_REQUEST] timedout: " << (parentTimeout < simTime()) << endl;
                //(parentTimeout < simTime())
            for (unsigned int i = 0; i < pStripe->Neighbors.size(); i++){
                int rnd = (int)intrand(pStripe->Neighbors.size());
                addr = pStripe->Neighbors.at(rnd);

                info = m_Base->getPeerInformation(addr);
                if ( (info == NULL) || (!info->isBoneNode(stripe)) ) // no peer information or peer is not a bone node?
                    continue;

                requestParentShip(stripe, addr);
                break;
            }
        }

    }

    // just to be sure ...
    m_Base->setReservedBandwidthFactor( totalChildren / (double)MTreeBoneSettings::theSettings->getNumberOfStripes() );
}

void MTreeBoneTree::handleMessage(cMessage *msg){
    if (msg->isSelfMessage())
    {
        //handleTimerMessage(msg);
    }
    else
    {
        MTreeBonePacketTree* csp = dynamic_cast<MTreeBonePacketTree*>(PK(msg));
        if (csp == NULL) return;

        IPvXAddress src = check_and_cast<DpControlInfo *>(PK(msg)->getControlInfo())->getSrcAddr();

        processPacket(csp, src);
        delete msg;
    }
}

void MTreeBoneTree::processPacket(MTreeBonePacketTree* pkt, IPvXAddress src){//cPacket *pkt){
    if (m_Base->hasLeftNetwork())
        return;

    int stripe;
    MTreeBoneStripeInformation* pStripe;
    IPvXAddress newParent;
    MTreeBonePeerInformation* info;

    m_Base->m_outFileDebug << "[DEBUG] received " << pkt->getPacketType() << endl;

    switch (pkt->getPacketType()){
        case MTREEBONE_PARENT_REQUEST:
            handleParentRequest(src, check_and_cast<MTreeBoneParentRequestPacket*> (pkt));
            break;
        case MTREEBONE_PARENT_REQUEST_RESPONSE:
            handleParentRequestResponse(src, check_and_cast<MTreeBoneParentRequestResponsePacket *> (pkt) );
            break;
        case MTREEBONE_PEER_INFORMATION:
            info = m_Base->getPeerInformation(src);
            if (info != NULL)
                for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
                    info->setDistance(i, (check_and_cast<MTreeBonePeerInformationPacket*> (pkt))->getDistance(i) );
                    info->setNumChildren(i, (check_and_cast<MTreeBonePeerInformationPacket*> (pkt))->getNumChildren(i) );
                }
            break;
        case MTREEBONE_SWITCH_POSITION_REQUEST:
            handleSwitchPositionRequest(src, check_and_cast<MTreeBonePeerSwitchPostionRequestPacket*> (pkt) );
            break;
        case MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE:
            handleSwitchPositionResponse(src, check_and_cast<MTreeBonePeerSwitchPostionRequestResponsePacket*> (pkt) );
            break;
        case MTREEBONE_REPLACE_CHILD:
            handleReplaceChild(src, check_and_cast<MTreeBonePeerReplaceChildPacket*> (pkt));
            break;
        case MTREEBONE_INFORM_NEW_PARENT:
            stripe = (check_and_cast<MTreeBonePeerInformNewParentPacket*> (pkt))->getStripe();
            pStripe = m_Base->getStripe(stripe);

            newParent = (check_and_cast<MTreeBonePeerInformNewParentPacket*> (pkt))->getNewParent();
            if (!newParent.equals(pStripe->Parent))
                removeParent(stripe);

            pStripe->Parent = newParent;
            m_Base->addNeighbor( newParent, stripe);

            if (m_Base->debugOutput)
                m_Base->m_outFileDebug << simTime().str() << " [INFORM_PARENT] got new parent " << newParent.str() << " for stripe " << stripe << endl;
            break;

        default:
        {
            //MTreeBoneBase::processPacket(pkt);

            break;
        }
    }
}

void MTreeBoneTree::removeParent(int stripe){
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);
    if (! pStripe->Parent.isUnspecified()){
        MTreeBoneParentRequestPacket* decline = new MTreeBoneParentRequestPacket();
        decline->setStripeNumber(stripe);
        decline->setAbort(true);
        m_Base->sendPacket(decline, pStripe->Parent);
        //sendToDispatcher(decline, m_localPort, m_Stripes[stripe].Parent, m_destPort);
        pStripe->Parent = IPvXAddress("0.0.0.0");
    }
}

void MTreeBoneTree::handleParentRequest(IPvXAddress src, MTreeBoneParentRequestPacket* pkt){
    int stripe = pkt->getStripeNumber();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    if (pkt->getAbort()){ // peer doesnt want to be our children anymore
        if (pStripe->Children.containsItem(src))
            m_Base->setReservedBandwidthFactor(m_Base->getReservedBandwidthFactor() - 1.0f / (double)MTreeBoneSettings::theSettings->getNumberOfStripes());
        pStripe->Children.removeItem(src);
        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [PARENTING] got remove from " << src.str() << " for stripe: " << stripe << endl;
        return;
    }

    MTreeBoneParentRequestResponsePacket* resp = new MTreeBoneParentRequestResponsePacket();
    resp->setStripeNumber(stripe);

    // same max number of children for all stripes TODO: think about making it 1 + x ?
    bool accepted = (m_Base->getRemainingBandwidthFactor() > 1) || (pStripe->Children.containsItem(src));
    accepted = accepted && m_Base->isBoneNodeForStripe(stripe);
    accepted = accepted && (m_Base->getVideoBuffer()->getBufferEndSeqNum() >= pkt->getOwnHead()); // only accept children which are not ahead of us

    resp->setIsAccepted( accepted );

    if ( (resp->getIsAccepted()) && (!(pStripe->Children.containsItem(src))) ){
        m_Base->addNeighbor(src, stripe);

        // set head in order to prevent duplicates
        MTreeBonePeerInformation* info = m_Base->getPeerInformation(src);
        info->setSequenceNumberEnd(pkt->getOwnHead());

        pStripe->Children.addItem(src);
        m_Base->setReservedBandwidthFactor(m_Base->getReservedBandwidthFactor() + 1.0f / (double)MTreeBoneSettings::theSettings->getNumberOfStripes());
        //checkFreeUploadListState();
    }

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [PARENTING] got request from " << src.str() << " for stripe: " << resp->getStripeNumber() << " status: "<< (resp->getIsAccepted() ? "accepted" : "denied") << " children: " << pStripe->Children.size() << endl;

    m_Base->sendPacket(resp, src);
    //sendToDispatcher(resp, m_localPort, src, m_destPort);
}
void MTreeBoneTree::handleReplaceChild(IPvXAddress src, MTreeBonePeerReplaceChildPacket* pkt){
    int stripe = pkt->getStripe();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    if (!pStripe->Children.containsItem(src))
        return; // a child wants to replace himself but he is not our child! oO

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [SWITCH] replacing child " << src.str() << " with " << pkt->getNewChild().str() << " for stripe " << stripe << endl;

    // remove current child
    pStripe->Children.removeItem(src);

    // add new child
    m_Base->addNeighbor(pkt->getNewChild(), stripe);
    pStripe->Children.addItem(pkt->getNewChild());
    //checkFreeUploadListState();

    // inform new child
    MTreeBonePeerInformNewParentPacket* inform = new MTreeBonePeerInformNewParentPacket();
    inform->setStripe(stripe);
    inform->setNewParent(m_Base->getAddress());

    m_Base->sendPacket(inform, pkt->getNewChild());
    //sendToDispatcher(inform, m_localPort, pkt->getNewChild(), m_destPort);

    // send distance to new child
    MTreeBonePeerInformationPacket* peerInfo = new MTreeBonePeerInformationPacket();
    peerInfo->setDistanceArraySize(MTreeBoneSettings::theSettings->getNumberOfStripes());
    peerInfo->setNumChildrenArraySize(MTreeBoneSettings::theSettings->getNumberOfStripes());
    for (unsigned int i = 0; i < MTreeBoneSettings::theSettings->getNumberOfStripes(); i++){
        peerInfo->setDistance(i, m_Base->getMyDistance(i));
        peerInfo->setNumChildren(i, m_Base->getReportedNumChildren(i) ); //m_Base->getStripe(i)->Children.size());
    }
    m_Base->sendPacket(peerInfo, pkt->getNewChild());
    //sendToDispatcher(peerInfo, m_localPort, pkt->getNewChild(), m_destPort);
}

void MTreeBoneTree::handleParentRequestResponse(IPvXAddress src, MTreeBoneParentRequestResponsePacket* resp){

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [PARENT] got response from " << src.str() << " for stripe: " << resp->getStripeNumber() << " status: "<< (resp->getIsAccepted() ? "accepted" : "denied") << endl;

    int stripe = resp->getStripeNumber();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    if (resp->getIsAccepted()){
        if (!pStripe->Parent.equals(src))
            removeParent(stripe);
        pStripe->Parent = src;
        parentTimeout = simTime() + 30;
        m_Base->addNeighbor(src, stripe);
    }else{
        if (pStripe->Parent.equals(src))
            pStripe->Parent = IPvXAddress("0.0.0.0");
        if (pStripe->Parent.isUnspecified())
            pStripe->nextParentRequest = simTime();
    }
}

void MTreeBoneTree::handleSwitchPositionRequest(IPvXAddress src, MTreeBonePeerSwitchPostionRequestPacket* req){
    int stripe = req->getStripe();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [SWITCH] got request from" << src.str() << " with suggested new parent " << req->getSuggestedNewParent().str() << " for stripe " << stripe << endl;

    if (m_Base->isSource()) // when we are source we cant switch ;)
        return;

    if (pStripe->Parent.isUnspecified()){
        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [SWITCH] DECLINED: NO PARENT" << endl;
        return;
    }

    MTreeBonePeerSwitchPostionRequestResponsePacket* resp = new MTreeBonePeerSwitchPostionRequestResponsePacket();
    resp->setStripe(stripe);
    resp->setNewParent(pStripe->Parent);

    // send switch
    MTreeBonePeerReplaceChildPacket* repChild = new MTreeBonePeerReplaceChildPacket();
    repChild->setNewChild(src);
    repChild->setStripe(stripe);

    m_Base->sendPacket(repChild, pStripe->Parent);
    //sendToDispatcher(repChild, m_localPort, m_Stripes[stripe].Parent, m_destPort);

    //addNeighbor(src, stripe);
    //m_Stripes[stripe].Parent = src;//IPAddress("0.0.0.0");
    pStripe->Parent = IPAddress("0.0.0.0");

    //m_Stripes[stripe].Parent = req->getNewParent();
    //addNeighbor(req->getNewParent(), stripe);

    // remove sender as a child ...
    removeChild(stripe, src);

    m_Base->sendPacket(resp, src);
    //sendToDispatcher(resp, m_localPort, src, m_destPort);

    // send a parent request to the suggested parent, if valid ...
    if (!req->getSuggestedNewParent().isUnspecified())
        requestParentShip(stripe, req->getSuggestedNewParent());

    //m_Stripes[stripe].nextParentRequest = 0;
    //checkParents();
}

void MTreeBoneTree::handleSwitchPositionResponse(IPvXAddress src, MTreeBonePeerSwitchPostionRequestResponsePacket* resp){
    int stripe = resp->getStripe();
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [SWITCH] got response from " << src.str() << " with new parent " << resp->getNewParent().str() << " for stripe " << stripe << endl;

    IPvXAddress newParent = resp->getNewParent();
    if (! pStripe->Parent.equals(newParent))
        removeParent(stripe);

    m_Base->addNeighbor(newParent, stripe);
    pStripe->Parent = newParent;
}

void MTreeBoneTree::removeChild(int stripe, IPvXAddress addr){

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [CHILD] remove child " << addr.str() << " for stripe: " << stripe << endl;
    m_Base->getStripe(stripe)->Children.removeItem(addr);

    MTreeBoneParentRequestResponsePacket* resp = new MTreeBoneParentRequestResponsePacket();
    resp->setStripeNumber(stripe);
    resp->setIsAccepted(false);

    m_Base->sendPacket(resp, addr);
    //sendToDispatcher(resp, m_localPort, addr, m_destPort);
}

void MTreeBoneTree::requestParentShip(int stripe, IPvXAddress addr){

    if (m_Base->getStripe(stripe)->Children.containsItem(addr))
        return;

    MTreeBoneParentRequestPacket* parentRequest = new MTreeBoneParentRequestPacket();
    parentRequest->setStripeNumber(stripe);
    parentRequest->setAbort(false);
    parentRequest->setOwnHead( m_Base->getOwnHead() );// getVideoBuffer()->getBufferEndSeqNum());

    m_Base->sendPacket(parentRequest, addr);
    //sendToDispatcher(parentRequest, m_localPort, addr, m_destPort);
    m_Base->getStripe(stripe)->nextParentRequest = simTime() + 3;
    //m_Stripes[stripe].nextParentRequest = simTime() + 3;

    if (m_Base->debugOutput)
        m_Base->m_outFileDebug << simTime().str() << " [PARENT] send request to " << addr.str() << " for stripe: "<< stripe << endl;
}

IPvXAddress MTreeBoneTree::findBetterParent(int stripe){
    MTreeBonePeerInformation* info;
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    int currentDistance = m_Base->getPeerInformation( pStripe->Parent )->getDistance(stripe);

    // check neighbors
    genericList<IPvXAddress> possible;
    std::vector<IPvXAddress>::iterator it;
    for (it = pStripe->Neighbors.begin(); it != pStripe->Neighbors.end(); it++){
        info = m_Base->getPeerInformation(*it);
        if (info == NULL) continue;

        if (    (info->getDistance(stripe) < currentDistance) &&
                (info->getDistance(stripe) >= 0) &&
                (info->getSequenceNumberEnd() > m_Base->getCurrentPlaybackPoint() + MTreeBoneSettings::theSettings->getChunksPerSecond())
           )
        {
            if (m_Base->debugOutput)
                m_Base->m_outFileDebug << simTime().str() << " [PARENTSELECTION]findBetterParent_a adding possible " << (*it).str() << " distance " << info->getDistance(stripe) << endl;
            possible.addItem(*it);
        }
    }

    std::vector<IPvXAddress> gossipAddr = m_Base->getGossiper()->getKnownPeers();
    for (it = gossipAddr.begin(); it != gossipAddr.end(); it++){
        GossipUserData* userData = m_Base->getGossiper()->getPeerData(*it);
        if (userData == NULL) continue;

        MTreeBoneGossipData* data = check_and_cast<MTreeBoneGossipData*> (userData);
        if ( (!data->getIsBoneNode(stripe)) || (data->getDistance(stripe) >= currentDistance) || (data->getDistance(stripe) < 0)){
            delete userData;
            continue;
        }

        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [PARENTSELECTION]findBetterParent_b adding possible " << (*it).str() << " distance " << data->getDistance(stripe) << endl;
        possible.addItem(*it);
        delete userData;
    }

    possible.removeItem( m_Base->getAddress() );

    if (possible.size() <= 0)
        return IPvXAddress("0.0.0.0");

    return possible.at( intrand(possible.size()) );
}

IPvXAddress MTreeBoneTree::findSwapCandidate(int stripe){
    MTreeBonePeerInformation* info;
    MTreeBoneStripeInformation* pStripe = m_Base->getStripe(stripe);

    IPvXAddress bestCandidate;
    int currentDistance = m_Base->getPeerInformation( pStripe->Parent )->getDistance(stripe);

    // check neighbors
    std::vector<IPvXAddress>::iterator it;
    for (it = pStripe->Neighbors.begin(); it != pStripe->Neighbors.end(); it++){
        info = m_Base->getPeerInformation(*it);
        if (info == NULL) continue;

        if (    (info->getDistance(stripe) < currentDistance) &&
                (info->getDistance(stripe) > 0) &&
                (info->getNumChildren(stripe) < m_Base->getReportedNumChildren(stripe) )// pStripe->Children.size())
                //(info->getSequenceNumberEnd() > mPlayer->getCurrentPlaybackPoint() + m_ChunksPerSecond) // shouldnt be important because we swap higher into the tree
           )
        {
            if (m_Base->debugOutput)
                m_Base->m_outFileDebug << simTime().str() << " [SWAPSELECTION]findSwapCandidate_a new best: " << (*it).str() << " distance " << info->getDistance(stripe) << endl;
            bestCandidate = (*it);
            currentDistance = info->getDistance(stripe);
        }
    }

    std::vector<IPvXAddress> gossipAddr = m_Base->getGossiper()->getKnownPeers();
    for (it = gossipAddr.begin(); it != gossipAddr.end(); it++){
        GossipUserData* userData = m_Base->getGossiper()->getPeerData(*it);
        if (userData == NULL) continue;

        MTreeBoneGossipData* data = check_and_cast<MTreeBoneGossipData*> (userData);
        if ( (!data->getIsBoneNode(stripe)) || (data->getDistance(stripe) >= currentDistance) || (data->getDistance(stripe) <= 0) || (data->getNumChildren(stripe) >= m_Base->getReportedNumChildren(stripe)) ){// pStripe->Children.size())){
            delete userData;
            continue;
        }

        if (m_Base->debugOutput)
            m_Base->m_outFileDebug << simTime().str() << " [SWAPSELECTION]findSwapCandidate_b new best: " << (*it).str() << " distance " << info->getDistance(stripe) << endl;

        bestCandidate = (*it);
        currentDistance = info->getDistance(stripe);

        delete userData;
    }

    return bestCandidate;
}

void MTreeBoneTree::initialize(int stage){
    if (stage != 3)
        return;

    param_DisablePush = par("disablePush");
}
