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

#ifndef SIMPLEGOSSIPINGTRACKERADAPTER_H_
#define SIMPLEGOSSIPINGTRACKERADAPTER_H_

class SimpleGossipingTrackerAdapter : public GossipProtocolWithUserData {
public:
    SimpleGossipingTrackerAdapter();
    virtual ~SimpleGossipingTrackerAdapter();


    // GossipProtocol
    virtual bool joinNetwork(IPvXAddress bootstrap = "0.0.0.0");
    virtual void leaveNetwork();
    virtual IPvXAddress getRandomPeer(IPvXAddress notThisAddress);
    virtual std::vector<IPvXAddress> getKnownPeers();
    virtual int requestUpdate();
    // GossipProtocolWithUserData
    virtual void setOwnData(GossipUserData* data);
    virtual GossipUserData* getPeerData(IPvXAddress addr);

    // returns a COPY
    GossipUserData* getOwnData(){ if (m_Data != NULL) return m_Data->dup(); return NULL;};

    IPvXAddress getAddress(){ return m_localAddress; };

    void triggerGossipDataReceived();
protected:
    virtual int numInitStages() const { return 5; }
    virtual void initialize(int stage);


    GossipUserData* m_Data;


    void removeOwnData(){if (m_Data != NULL) delete m_Data; m_Data = NULL;};
};

#endif /* SIMPLEGOSSIPINGTRACKERADAPTER_H_ */
