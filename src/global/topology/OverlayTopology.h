#ifndef OVERLAYTOPOLOGY_H
#define OVERLAYTOPOLOGY_H

#include "TopologyModel.h"

class OverlayTopology : public cSimpleModule {
    typedef std::map<int, TopologyModel>::iterator Iterator;
    typedef std::map<int, TopologyModel>           TopologyMap;
    typedef std::list<PPEdge>                       PPEdgeList;

private:
    TopologyMap        topo;            ///< the topology data structure < streamID, Topology >

public:
    void initialize();
    //virtual int numInitStages() const { return 5; }
    void finish();
    void handleMessage(cMessage* msg);

    void setRoot(const IPvXAddress& root, const int sequence);
    void addNode(const IPvXAddress& ip, const int sequence);  ///< for hierarchy per sequence
    void removeNode(const IPvXAddress& ip, const int sequence);

    void addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to); ///< for hierarchy per sequence
    void removeEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to);

    int           getNumVerticesSequence(const int sequence);
    int           getNumTopologies();

    // topology per packet
    TopologyModel      getTopology();
    TopologyModel      getTopology(const int sequence);              ///< for hierarchy per sequence
    TopologyModel&     getTopologyRef();
    TopologyModel&     getTopologyRef(const int sequence);              ///< for hierarchy per sequence
    TopologyModel      getMostRecentTopology();
    TopologyModel&     getMostRecentTopologyRef();
    PPEdgeList         getEdges(const int sequence);
    void  setJoinTime(const int sequence, IPvXAddress node, double joinTime);
    void  setJoinTime(IPvXAddress node, double joinTime);

    int getMostRecentSeq();
    int getMaxRecentSeq();

    int attackRecursive(const int num);
    int attackRecursive(const int sequence, const int num);
};

#endif // OVERLAYTOPOLOGY_H
