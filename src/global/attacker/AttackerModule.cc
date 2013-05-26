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
// AttackerModule.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Mathias Fischer;
// Code Reviewers: Giang Nguyen;
// -----------------------------------------------------------------------------
//

#include "AttackerModule.h"
#include "OverlayTopology.h"
#include "TopologyModel.h"

using namespace std;

Define_Module(AttackerModule);

AttackerModule::~AttackerModule()
{
   if (timer_attack) cancelAndDelete(timer_attack);
}

/**
 * initialise the AttackerModule in Stage 5
 */
void AttackerModule::initialize(int stage) {

   if (stage != 4) return;

   //FIXME get pointer to OverlayTopology-module
   // --> will be done in the derived class AttackerModuleDonet
   //   oT = NULL;

   timer_attack = new cMessage("ATTACKER_MODULE_TRIGGER");

   // the global attacker parameters
   startAttack = par("start").doubleValue();
   stopAttack = par("stop").doubleValue();
   assert(startAttack < stopAttack);
   interval = par("interval").doubleValue();

   // Percentage of nodes to attack
   //percentage = par("percentage").doubleValue(); // not used so far
   numAttack = par("numAttack").longValue();

   // the statistical parameters
   statname_damage_global = "streaming_importance_global";

   // init statistics
   initStatistics();

   // schedule attacks+statistics
   //scheduleAt(simTime() + startAttack, new cMessage("GLOBAL_SCHED_ID"));
   scheduleAt(simTime() + startAttack, timer_attack);
   updateDisplay();

}

//TopologyModel AttackerModule::getTopo(const int sequence) {

//   return oT->getTopology(sequence);
//}


void AttackerModule::finish() {

   // *** Statistics ***
   char buffer[128];
   // - write damage to sca
   sprintf(buffer, "%s - %s", "attacker.damage", "mean");
   recordScalar(buffer, attackerDamage.getMean());
   sprintf(buffer, "%s - %s", "attacker.damage", "min");
   recordScalar(buffer, attackerDamage.getMin());
   sprintf(buffer, "%s - %s", "attacker.damage", "max");
   recordScalar(buffer, attackerDamage.getMax());
}

void AttackerModule::handleMessage(cMessage* msg) {

   if (!msg->isSelfMessage()) {
      delete msg;
      msg = NULL;
      throw cRuntimeError("AttackerModule cannot handle non-loopback messages");

   }

   attackGlobal();

   if (simTime() + interval <= stopAttack) {
      scheduleAt(simTime() + interval, timer_attack);
      msg = NULL;
      return;

   } else {
      delete msg;
      msg = NULL;
   }

   assert(msg == NULL);
}


void AttackerModule::attackGlobal() {

   // put stats here
   int damage = oT->attackRecursive(numAttack);

   //FIXME recording the damage
   attackerDamage.collect((double) damage);
   attackerDamageTime.record((double) damage);
}




/**
 * starts statistics
 */
void AttackerModule::initStatistics() {
   nodesOverallVec.setName("total_number_nodes");
}

/**
 * omnetpp standard method
 *
 * There are two kinds of messages to come in:
 *   - loopback: ATTACK_MSG_ID: triggers the global attacker
 *   - loopback: AGENT_MSG_ID: triggers the attack agent
 */
void AttackerModule::updateDisplay() {
#if _DEBUG
   char buffer[32];
   sprintf(buffer, "%d getNode(s)", (int)modules.size());

   cDisplayString display = getDisplayString();
   // to show the current amount of nodes
   display.setTagArg("t", 0, buffer);
   display.setTagArg("t", 1, "t");
   display.setTagArg("t", 2, "darkgray");
   // and also the online status
   setDisplayString(display.str());
#endif
}

