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
// ForwarderTopo.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//




#ifndef FORWARDERTOPO_H_
#define FORWARDERTOPO_H_

#include "Forwarder.h"
#include "OverlayTopologyDonet.h"

// @brief reports source & dest of a chunk speicified by observedChunk
class ForwarderTopo : public Forwarder
{
public:
   ForwarderTopo();
   virtual ~ForwarderTopo();

   virtual void handleMessage(cMessage* msg);
   virtual int numInitStages() const { return 4; }
   virtual void initialize(int stage);
   virtual void finish();

protected:
   // -- Pointers to the global modules
   OverlayTopologyDonet* m_topoObserver;

   // @brief stores the seq_num of the chunk to be observed
   static long m_observedChunk;
   // @brief stores the sequence of the overlay to be observed
   static int m_topoSequence;
};

#endif /* FORWARDERTOPO_H_ */
