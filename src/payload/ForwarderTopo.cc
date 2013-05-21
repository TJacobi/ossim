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
// ForwarderTopo.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "ForwarderTopo.h"
#include "DpControlInfo_m.h"

Define_Module(ForwarderTopo)

long ForwarderTopo::m_observedChunk;
int ForwarderTopo::m_topoSequence;

ForwarderTopo::ForwarderTopo() {}

ForwarderTopo::~ForwarderTopo() {}

void ForwarderTopo::initialize(int stage)
{
   Forwarder::initialize(stage);

   if (stage != 3)
      return;

   cModule *temp = simulation.getModuleByPath("topoObserver");
   m_topoObserver = check_and_cast<OverlayTopologyDonet *>(temp);

   m_observedChunk = m_topoObserver->getObservedChunk();
   m_topoSequence = m_observedChunk % INT_MAX;

   WATCH(m_topoObserver);
   WATCH(m_observedChunk);
   WATCH(m_topoSequence);
}

void ForwarderTopo::finish()
{
}

void ForwarderTopo::handleMessage(cMessage* msg)
{
   Enter_Method("handleMessage");

   if (msg->isSelfMessage())
   {
      throw cException("No timer is used in this module");
      return;
   }

   // -------------------------------------------------------------------------
   // -- For incoming chunks
   // -------------------------------------------------------------------------

   IPvXAddress senderAddress;
   DpControlInfo *controlInfo = check_and_cast<DpControlInfo *>(msg->getControlInfo());
//   senderAddress = controlInfo->getSrcAddr();

   PeerStreamingPacket *appMsg = check_and_cast<PeerStreamingPacket *>(msg);
   EV << "PacketGroup = " << appMsg->getPacketGroup() << endl;
   if (appMsg->getPacketGroup() != PACKET_GROUP_VIDEO_CHUNK)
      throw cException("Wrong packet type!");

//   EV << "A video chunk has just been received from " << senderAddress << endl;
   VideoChunkPacket *chunkPkt = check_and_cast<VideoChunkPacket *>(appMsg);

   m_videoBuffer->insertPacket(chunkPkt);

   // -- Local record of incoming chunks
   updateReceivedChunkRecord(senderAddress);
   ++m_count_totalChunk_incoming;

   // -- Report to the topology observer
   long chunkSeqNum = chunkPkt->getSeqNumber();
   EV << "!!!observing!!!" << endl;
   EV << "current chunk: " << chunkSeqNum << " -- expected one: " << m_observedChunk << endl;
   if (chunkSeqNum == m_observedChunk)
   {
      m_topoObserver->addNode(controlInfo->getDestAddr(), m_topoSequence);
      m_topoObserver->addEdge(m_topoSequence, controlInfo->getSrcAddr(), controlInfo->getDestAddr());
      EV << "addNode & addEdge" << endl;
   }
}
