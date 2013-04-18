/*
 * InetUnderlayConfigurator.h
 *
 *  Created on: Jul 9, 2009
 *      Author: mtrapp,mathias
 */

#ifndef INETUNDERLAYCONFIGURATOR_H_
#define INETUNDERLAYCONFIGURATOR_H_

//#include "p2pstreaming.h"
#include "INETDefs.h"
#include "IPAddress.h"
#include <map>
//#include "../networklayer/autorouting/FlatNetworkConfigurator.h"
#include "../../../inet/src/networklayer/autorouting/FlatNetworkConfigurator.h"

class IInterfaceTable;
class IRoutingTable;


class InetUnderlayConfigurator: public FlatNetworkConfigurator {

private:
    std::vector<cModule*> accessRouter;
    std::vector<std::pair<cModule*,double> > accessRoutersWeighted;

    // Access delay bounds for clients
    double minAccessDelay;
    double maxAccessDelay;
public:
    cTopology topo;
    NodeInfoVector nodeInfo; // will be of size topo.nodes[]
    std::map<SimTime, IPAddress> jumps;
    std::set<cModule*> nodeList;
    int numIPNodes;
    int numNodes;
    int numRouter;
    int numTerminals;
    int numSources;
    int maxNumSources;
    IInterfaceTable *oldIT;
    bool noneTransitNodesAccessRouters;

public:
	void addDefaultRoutes(cTopology& topo, NodeInfoVector& nodeInfo, cModule *jumpedNode);
	void addDefaultRoutes(cTopology& topo, NodeInfoVector& nodeInfo);
	void addDefaultRoutes();
	void assignNewAddress(IPAddress oldAddress, int interfaceNbr);
	void connect(cModule* terminal,bool conn);
	void deleteOutdatedRoutes();
	void drop1stInterfaceEntry(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode, cModule *ppp);
    void fillRoutingTables(cTopology& topo, NodeInfoVector& nodeInfo, cModule *jumpedNode);
    void fillRoutingTables(cTopology& topo, NodeInfoVector& nodeInfo);
    void fillRoutingTables(cModule* terminal, IPAddress destAddr);
    void getSecondAddress(IPAddress oldAddress, int interfaceNbr);
    void reAdd1stInterfaceEntry(cTopology& topo, NodeInfoVector& nodeInfo, cModule *overlayNode, cModule *ppp);
	bool updateRoutes(cModule *jumpedNode, bool multihoming, cModule *modulePPP);

protected:
    IPAddress assignAddress(cModule* terminal);
    void assignAddresses(cTopology& topo, NodeInfoVector& nodeInfo);
    cModule* createNode(bool conn);
    void kfournisseur();
    void initNodes();
    void initSources();
    void getAccessRouter();
	// TODO
	// removeOverlayNode(cModule* terminal);
	void checkRessources();

public: // OMNeT++ stuff
    virtual int numInitStages() const { return 6; }
    void initialize(int stage);
    void handleMessage(cMessage* msg);
	virtual void finish();

};

#endif /* INETUNDERLAYCONFIGURATOR_H_ */
