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

#include "GossipingTracker.h"
//#include "GenericList.h"

GossipingTracker* GossipingTracker::theTracker = NULL; // global pointer for easier acces

Define_Module(GossipingTracker)

GossipingTracker::GossipingTracker() {
    m_Timer_GossipDataReceived  = NULL;
    m_Timer_SwitchPeers         = NULL;
}

GossipingTracker::~GossipingTracker() {
    if (m_Timer_GossipDataReceived != NULL) cancelAndDelete(m_Timer_GossipDataReceived); m_Timer_GossipDataReceived = NULL;
    if (m_Timer_SwitchPeers != NULL) cancelAndDelete(m_Timer_SwitchPeers); m_Timer_SwitchPeers = NULL;

}

void GossipingTracker::initialize(int stage){

    if (stage !=2)
        return;

    GossipingTracker::theTracker = this;

    param_numKnownPeers                 = par("numKnownPeers");
    param_GossipDataReceived_Intervall  = par("GossipDataReceivedIntervall");

    m_Timer_GossipDataReceived = new cMessage("GOSSIPING_TRACKER_GOSSIP_DATA_RECEIVED");
    scheduleAt(simTime() + param_GossipDataReceived_Intervall, m_Timer_GossipDataReceived);

    m_Timer_SwitchPeers = new cMessage("GOSSIPING_TRACKER_SWITCH_PEERS");
    scheduleAt(simTime() + param_GossipDataReceived_Intervall, m_Timer_SwitchPeers);

}

void GossipingTracker::handleMessage(cMessage *msg){
    if (!msg->isSelfMessage())
        return;

    if (msg == m_Timer_GossipDataReceived){

        std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.begin();

        while ( it != mPeers.end() ){
            (*it).first->triggerGossipDataReceived();
            it++;
        }
        /*
        genericList<SimpleGossipingTrackerAdapter*>::iterator it = mPeers.begin();
        while (it != mPeers.end()){
            (*it)->triggerGossipDataReceived();
            it++;
        }*/

        scheduleAt(simTime() + param_GossipDataReceived_Intervall, m_Timer_GossipDataReceived);
    }else if(msg == m_Timer_SwitchPeers){

        shufflePeers();

        scheduleAt(simTime() + param_GossipDataReceived_Intervall, m_Timer_SwitchPeers);
    }
}

void GossipingTracker::shufflePeers(){
    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.begin();

    while ( it != mPeers.end() ){
        SimpleGossipingTrackerAdapter* rnd = getRandomPeer( (*it).first );
        if ((rnd == NULL) || ((*it).second.containsItem(rnd))){

        }else{  // add the new random peer ... replace one if necessary
            if ( (*it).second.size() >= param_numKnownPeers )
                (*it).second.removeItem( (*it).second.getRandomItem() );

            (*it).second.push_back(rnd);
        }
        it++;
    }
}

void GossipingTracker::shufflePeersKnownPeers(SimpleGossipingTrackerAdapter* gossiper){
    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.find( gossiper );
    if (it == mPeers.end()) return ;

    (*it).second.clear();
    for (int i = 0; i < param_numKnownPeers; i++){
        SimpleGossipingTrackerAdapter* rnd = getRandomPeer( (*it).first );

        if (rnd != NULL)
            (*it).second.addItem(rnd);
    }
}
void GossipingTracker::joinNetwork(SimpleGossipingTrackerAdapter* gossiper){
    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.find( gossiper );
    if (it != mPeers.end()) return ;

    genericList<SimpleGossipingTrackerAdapter*> list;
    mPeers.insert(std::pair< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> > (gossiper, list));

    shufflePeersKnownPeers(gossiper);
    //mPeers.addItem(gossiper);
}
void GossipingTracker::leaveNetwork(SimpleGossipingTrackerAdapter* gossiper){
    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.find( gossiper );
    if (it == mPeers.end()) return ;

    mPeers.erase(it);
    //mPeers.removeItem(gossiper);
}

SimpleGossipingTrackerAdapter* GossipingTracker::getRandomPeer(SimpleGossipingTrackerAdapter* exclude){
    // since this is just a simple implementation return a random peer of all joined peers ...
    if (mPeers.empty())
        return NULL;

    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.begin();


    if ( ((*it).first == exclude) && (mPeers.size() == 1) )
        return NULL;

    while (true){
        it = mPeers.begin();
        std::advance( it, (int)intrand(mPeers.size()));

        if ( (*it).first != exclude )
            return (*it).first;
    }

    return NULL;

    /*
    if ( addr.equals(notThisAddress) && (mPeers.size() == 1))
        return "0.0.0.0";

    // loop until we find one that isnt ours ...
    while(addr.equals(notThisAddress)){
        it = mPeers.begin();
        std::advance( it, (int)intrand(mPeers.size()));
        addr = it->first;
    }

    return addr;

    if (mPeers.size() == 0)
        return NULL;

    std::map<SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*>>::iterator it = mPeers.begin();*/
}

SimpleGossipingTrackerAdapter* GossipingTracker::getRandomPeerForPeer(SimpleGossipingTrackerAdapter* requester){
    // since this is just a simple implementation return a random peer of all joined peers ...
    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.find( requester );
    if (it == mPeers.end()) return NULL;

    if ((*it).second.empty()) return NULL;

    return (*it).second.getRandomItem();


    /*if (mPeers.size() == 0) // none available :(
        return NULL;

    SimpleGossipingTrackerAdapter* ret = mPeers.getRandomItem();
    if ( (ret == requester) && (mPeers.size() == 1))    // only requester available :(
        return NULL;

    while( (ret == requester) ){
        ret = mPeers.getRandomItem();
    }

    return ret;*/
}

genericList<SimpleGossipingTrackerAdapter*> GossipingTracker::getKnownPeersForPeer(SimpleGossipingTrackerAdapter* requester){
    genericList<SimpleGossipingTrackerAdapter*> ret;

    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.find( requester );
    if (it == mPeers.end())
        return ret;

    return (*it).second;
}

// searches for a peer in a peers known nodes list
SimpleGossipingTrackerAdapter* GossipingTracker::getPeerByAddress(SimpleGossipingTrackerAdapter* requester, IPvXAddress addr){
    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> >::iterator it = mPeers.find( requester );
    if (it == mPeers.end()) return NULL;

    genericList<SimpleGossipingTrackerAdapter*>::iterator lIt = (*it).second.begin();
    while ( lIt != (*it).second.end() ){
        if ( (*lIt)->getAddress().equals(addr) ) return (*lIt);
        lIt++;
    }

    return NULL;
    /*
    if (! (*it).second.containsItem())
    genericList<SimpleGossipingTrackerAdapter*>::iterator it = mPeers.begin();
    while ( it != mPeers.end()){
        if ( (*it)->getAddress().equals(addr) )
            return (*it);
        it++;
    }

    return NULL;*/
}


/*
 *
// GossipProtocol
bool SimpleGossipingTracker::joinNetwork(IPvXAddress localaddress, IPvXAddress bootstrap = "0.0.0.0"){
    if (findPeer(localaddress) != NULL) return true;

    mPeers.insert( std::pair<IPvXAddress, SimpleGossipingTrackerData*> (localaddress, new SimpleGossipingTrackerData()) );
    return true;
}
void SimpleGossipingTracker::leaveNetwork(IPvXAddress localaddress){
    std::map<IPvXAddress, SimpleGossipingTrackerData*>::iterator it = mPeers.find( localaddress );
    if (it == mPeers.end()) return;

    delete (*it).second;
    mPeers.erase( it );
}


IPvXAddress SimpleGossipingTracker::getRandomPeer(IPvXAddress localaddress, IPvXAddress notThisAddress){
    // since this is just a simple implementation return a random peer of all joined peers ...
    if (mPeers.size() == 0)
        return "0.0.0.0";

    std::map<IPvXAddress, SimpleGossipingTrackerData*>::iterator it = mPeers.begin();

    IPvXAddress addr = it->first;

    // if this address is the one to be ignored and there is only one entry we cant do anything :(
    if (addr.equals(notThisAddress) && (mPeers.size() == 1))
        return "0.0.0.0";

    // loop until we find one that isnt ours ...
    while(addr.equals(notThisAddress)){
        it = mPeers.begin();
        std::advance( it, (int)intrand(mPeers.size()));
        addr = it->first;
    }

    return addr;
}
std::vector<IPvXAddress> SimpleGossipingTracker::getKnownPeers(IPvXAddress localaddress){
    genericList<IPvXAddress> ret;
    //std::vector<IPvXAddress> ret;
    int tries = param_numKnownPeers * 2;
    IPvXAddress addr;

    while ( (ret.size() < param_numKnownPeers) && (tries > 0) ){
        addr = getRandomPeer(localaddress, localaddress);
        if (!addr.isUnspecified()) ret.addItem(addr);
    }

    return ret;
}

//GossipProtocolWithUserData
void SimpleGossipingTracker::setOwnData(IPvXAddress localaddress, GossipUserData* data){
    SimpleGossipingTrackerData* ptr = findPeer(localaddress);
    if (ptr == NULL) return;

    ptr->setData(data);
}
GossipUserData* SimpleGossipingTracker::getPeerData(IPvXAddress localaddress, IPvXAddress addr){
    SimpleGossipingTrackerData* ptr = findPeer(localaddress);
    if (ptr == NULL) return NULL;

    return ptr->getData();
}

SimpleGossipingTrackerData* SimpleGossipingTracker::findPeer(IPvXAddress addr){
    std::map<IPvXAddress, SimpleGossipingTrackerData*>::iterator it = mPeers.find( addr );
    if (it == mPeers.end()) return NULL;

    return it->second;
}
*/
