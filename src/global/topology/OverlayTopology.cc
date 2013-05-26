#include "OverlayTopology.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;

Define_Module(OverlayTopology);


void OverlayTopology::initialize() {
}

void OverlayTopology::finish() {
    topo.clear();
}

void OverlayTopology::handleMessage(cMessage* msg) {
	throw cRuntimeError("OverlayTopology::handleMessage");
}


/**
 * Set this node as root in the respective TopologyModels
 *
 * @param root  The IP of this root
 * @param sequence The respective sequence number
 */
void OverlayTopology::setRoot(const IPvXAddress & root, const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    if(topo.find(sequence) != topo.end())
        topo[sequence].setRoot(root);
}


void OverlayTopology::removeEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].removeEdge(from,to);
}


int OverlayTopology::getNumTopologies() {

	return topo.size();
}


void OverlayTopology::addNode(const IPvXAddress& ip, const int sequence) {
    Enter_Method_Silent();

    topo[sequence].addVertex(ip);
}

void OverlayTopology::removeNode(const IPvXAddress& ip, const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].removeVertex(ip);
}


/**
  * \param add the edge
 */
void OverlayTopology::addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].addEdge(from, to);
}


int OverlayTopology::getNumVerticesSequence(const int sequence) {
    Enter_Method_Silent();

    if (topo.count(sequence) == 1) {
        return topo[sequence].numberVertexes();
    } else {
        return 0;
    }
}


void OverlayTopology::setJoinTime(const int sequence, IPvXAddress node, double joinTime) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    topo[sequence].setAge(node, joinTime);
}


PPEdgeList OverlayTopology::getEdges(const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    return topo[sequence].getEdges();
}


TopologyModel OverlayTopology::getMostRecentTopology() {

    int topoNum = getMaxRecentSeq();
    return getTopology(topoNum);
}


TopologyModel OverlayTopology::getTopology() {

    return getMostRecentTopology();
}

TopologyModel & OverlayTopology::getTopologyRef() {
    int topoNum = getMaxRecentSeq();
    return getTopologyRef(topoNum);
}

TopologyModel& OverlayTopology::getMostRecentTopologyRef(){
    int topoNum = getMaxRecentSeq();
    return getTopologyRef(topoNum);
}

TopologyModel OverlayTopology::getTopology(const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    return topo[sequence];
}

TopologyModel& OverlayTopology::getTopologyRef(const int sequence) {
    Enter_Method_Silent();

    assert(topo.find(sequence) != topo.end());
    return topo[sequence];
}


int OverlayTopology::attackRecursive(const int num) {

    // get most recent sequence number
    int sequence = getMaxRecentSeq();
    return attackRecursive(sequence, num);
}


int OverlayTopology::getMostRecentSeq() {

    int seq = -1;
    for(Iterator it = topo.begin(); it != topo.end(); it++) {
        if(seq < it->first) seq = it->first;
    }
    assert(seq >= 0);
    return seq;
}

// returns sequence number of topology
// with maximum number of nodes
int OverlayTopology::getMaxRecentSeq() {
   int seq = -1;
   int max = -1;
   for(Iterator it = topo.begin(); it != topo.end(); it++) {
       int num = it->second.numberVertexes();
       if(max < num || (max == num && seq < it->first)) {
           seq = it->first;
           max = num;
       }
   }
   assert(seq >= 0);
   return seq;
}


int OverlayTopology::attackRecursive(const int sequence, const int num) {

    TopologyModel topoM = getTopology(sequence);

    int damage = 0;
    for(int i = 0; i < num; i++) damage += topoM.removeCentralVertex();

    return damage;
}
