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

#include "csimplemodule.h"
#include "IPvXAddress.h"
#include "GossipProtocolWithUserData.h"
#include "SimpleGossipingTrackerAdapter.h"

#ifndef SIMPLEGOSSIPINGTRACKER_H_
#define SIMPLEGOSSIPINGTRACKER_H_

class GossipingTracker : public cSimpleModule{
public:
    GossipingTracker();
    virtual ~GossipingTracker();

    void joinNetwork(SimpleGossipingTrackerAdapter* gossiper);
    void leaveNetwork(SimpleGossipingTrackerAdapter* gossiper);

    SimpleGossipingTrackerAdapter* getRandomPeer(SimpleGossipingTrackerAdapter* exclude);
    SimpleGossipingTrackerAdapter* getRandomPeerForPeer(SimpleGossipingTrackerAdapter* requester);
    SimpleGossipingTrackerAdapter* getPeerByAddress(SimpleGossipingTrackerAdapter* requester, IPvXAddress addr);
    genericList<SimpleGossipingTrackerAdapter*> getKnownPeersForPeer(SimpleGossipingTrackerAdapter* requester);

    void shufflePeersKnownPeers(SimpleGossipingTrackerAdapter* target);

    int inline getParameter_NumKnownPeers(){return param_numKnownPeers;};


    void handleMessage(cMessage *msg);

    static GossipingTracker* theTracker; // global pointer for easier acces
protected:
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);

    std::map< SimpleGossipingTrackerAdapter*, genericList<SimpleGossipingTrackerAdapter*> > mPeers;
    void shufflePeers();
    //genericList<SimpleGossipingTrackerAdapter*> mPeers;

    int     param_numKnownPeers;
    double  param_GossipDataReceived_Intervall;

    cMessage* m_Timer_GossipDataReceived;
    cMessage* m_Timer_SwitchPeers;
};

#endif /* SIMPLEGOSSIPINGTRACKER_H_ */
