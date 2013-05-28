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
// AttackerModuleDonet.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "AttackerModuleDonet.h"
#include "OverlayTopologyDonet.h"
//#include "TopologyModel.h"

using namespace std;

Define_Module(AttackerModuleDonet);

/**
 * initialise the AttackerModule in Stage 3.
 */
void AttackerModuleDonet::initialize(int stage)
{
   AttackerModule::initialize(stage);

   if (stage != 4)
      return;

   // -- Binding to the global module Topology Observer
   cModule *temp = simulation.getModuleByPath("topoObserver");
   oT = check_and_cast<OverlayTopologyDonet *>(temp);

//   if (stage != 3) return;


//   // the global attacker parameters
//   startAttack = par("start");
//   stopAttack = par("stop");
//   assert(startAttack < stopAttack);
//   interval = par("interval");

//   // Percentage of nodes to attack
//   percentage = par("percentage");
//   numAttack = par("numAttack");

//   // the statistical parameters
//   statname_damage_global = "streaming_importance_global";

//   // init statistics
//   initStatistics();

//   // schedule attacks+statistics
//   scheduleAt(simTime() + startAttack, new cMessage("GLOBAL_SCHED_ID"));
//   updateDisplay();

}

//TopologyModel AttackerModule::getTopo(const int sequence) {

//   return oT->getTopology(sequence);
//}


void AttackerModuleDonet::finish()
{
   AttackerModule::finish();

//   // *** Statistics ***
//   char buffer[128];
//   // - write damage to sca
//   sprintf(buffer, "%s - %s", "attacker.damage", "mean");
//   recordScalar(buffer, attackerDamage.getMean());
//   sprintf(buffer, "%s - %s", "attacker.damage", "min");
//   recordScalar(buffer, attackerDamage.getMin());
//   sprintf(buffer, "%s - %s", "attacker.damage", "max");
//   recordScalar(buffer, attackerDamage.getMax());
}

void AttackerModuleDonet::handleMessage(cMessage* msg)
{
   AttackerModule::handleMessage(msg);

//   if (!msg->isSelfMessage()) {
//      delete msg;
//      msg = NULL;
//      throw cRuntimeError("AttackerModule cannot handle non-loopback messages");
//   }

//   attackGlobal();

//   if (simTime() + interval <= stopAttack) {
//      scheduleAt(simTime() + interval, msg);
//      msg = NULL;
//      return;

//   } else {
//      delete msg;
//      msg = NULL;
//   }

//   assert(msg == NULL);
}


//void AttackerModuleDonet::attackGlobal()
//{
//   AttackerModule::attackGlobal();

//}


/**
 * starts statistics
 */
//void AttackerModule::initStatistics() {
//   nodesOverallVec.setName("total_number_nodes");
//}

/**
 * omnetpp standard method
 *
 * There are two kinds of messages to come in:
 *   - loopback: ATTACK_MSG_ID: triggers the global attacker
 *   - loopback: AGENT_MSG_ID: triggers the attack agent
 */
//void AttackerModule::updateDisplay() {
//#if _DEBUG
//   char buffer[32];
//   sprintf(buffer, "%d getNode(s)", (int)modules.size());

//   cDisplayString display = getDisplayString();
//   // to show the current amount of nodes
//   display.setTagArg("t", 0, buffer);
//   display.setTagArg("t", 1, "t");
//   display.setTagArg("t", 2, "darkgray");
//   // and also the online status
//   setDisplayString(display.str());
//#endif
//}


//TopologyModel AttackerModuleDonet::getTopo(const int sequence) {

//   return oT->getTopology(sequence);
//}
