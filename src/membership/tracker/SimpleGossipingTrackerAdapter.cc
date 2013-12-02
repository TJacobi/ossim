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

#include "SimpleGossipingTrackerAdapter.h"
#include "GossipingTracker.h"

Define_Module(SimpleGossipingTrackerAdapter)

SimpleGossipingTrackerAdapter::SimpleGossipingTrackerAdapter() {
    // TODO Auto-generated constructor stub
    m_Data = NULL;
}

SimpleGossipingTrackerAdapter::~SimpleGossipingTrackerAdapter() {
    // TODO Auto-generated destructor stub
    removeOwnData();
}

void SimpleGossipingTrackerAdapter::initialize(int stage){
    if (stage != 3)
        return;

    findNodeAddress();
}

// GossipProtocol
bool SimpleGossipingTrackerAdapter::joinNetwork(IPvXAddress bootstrap){
    GossipingTracker::theTracker->joinNetwork(this);
    return true;
}
void SimpleGossipingTrackerAdapter::leaveNetwork(){
    GossipingTracker::theTracker->leaveNetwork(this);
}

IPvXAddress SimpleGossipingTrackerAdapter::getRandomPeer(IPvXAddress notThisAddress){
    SimpleGossipingTrackerAdapter* peer = GossipingTracker::theTracker->getRandomPeerForPeer(this);
    if (peer == NULL) return IPvXAddress("0.0.0.0");

    for (int i = 0; i < 5; i++){
        if (!peer->getAddress().equals(notThisAddress)) return peer->getAddress();
        peer = GossipingTracker::theTracker->getRandomPeerForPeer(this);
        if (peer == NULL) return IPvXAddress("0.0.0.0");
    }

    return IPvXAddress("0.0.0.0");//peer->getAddress();
}

std::vector<IPvXAddress> SimpleGossipingTrackerAdapter::getKnownPeers(){

    genericList<SimpleGossipingTrackerAdapter*> list = GossipingTracker::theTracker->getKnownPeersForPeer(this);
    genericList<IPvXAddress> ret;

    genericList<SimpleGossipingTrackerAdapter*>::iterator it = list.begin();
    while (it != list.end()){
        ret.addItem( (*it)->getAddress() );
        it++;
    }

    return ret;

    /*
    int param_numKnownPeers = GossipingTracker::theTracker->getParameter_NumKnownPeers();
    int tries = param_numKnownPeers * 2;
    IPvXAddress addr;

    while ( (ret.size() < param_numKnownPeers) && (tries > 0) ){
        addr = getRandomPeer(m_localAddress);
        if (addr.isUnspecified()) break;
        //if (!addr.isUnspecified())
        ret.addItem(addr);
    }

    return ret;*/
}

// GossipProtocolWithUserData
void SimpleGossipingTrackerAdapter::setOwnData(GossipUserData* data){
    removeOwnData();
    if (data != NULL) m_Data = data->dup();
}
GossipUserData* SimpleGossipingTrackerAdapter::getPeerData(IPvXAddress addr){
    SimpleGossipingTrackerAdapter* peer = GossipingTracker::theTracker->getPeerByAddress(this, addr);

    if (peer != NULL)
        return peer->getOwnData();
    return NULL;
}


void SimpleGossipingTrackerAdapter::triggerGossipDataReceived(){
    genericList<GossipListener*>::iterator iter;

    for (iter = m_GossipListener.begin(); iter != m_GossipListener.end(); iter++){
        (*iter)->onGossipDataReceived();
    }
}
