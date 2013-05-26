//
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// OverlayTopology.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#ifndef OVERLAYTOPOLOGY_H
#define OVERLAYTOPOLOGY_H

#include "TopologyModel.h"
#include "AppSetting.h"

class OverlayTopology : public cSimpleModule {
    typedef std::map<int, TopologyModel>::iterator Iterator;
    typedef std::map<int, TopologyModel>           TopologyMap;
    typedef std::list<PPEdge>                       PPEdgeList;

protected:
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
    TopologyModel      getSequenceTopology(const int sequence);              ///< for hierarchy per sequence
    TopologyModel&     getSequenceTopologyRef(const int sequence);              ///< for hierarchy per sequence
    PPEdgeList         getEdges(const int sequence);

    void  setJoinTime(const int sequence, IPvXAddress node, double joinTime);
    void  setJoinTime(IPvXAddress node, double joinTime);

    // -- Interface to Attacker module
    TopologyModel getTopology(int sequence) {}
//     virtual TopologyModel getTopology(int sequence) = 0;

};

#endif // OVERLAYTOPOLOGY_H
