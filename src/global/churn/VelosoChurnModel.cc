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
// VelosoChurnModel.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//


#include "VelosoChurnModel.h"

Define_Module(VelosoChurnModel);

double VelosoChurnModel::m_absoluteInterval = 0.0;

VelosoChurnModel::VelosoChurnModel() {
    // TODO Auto-generated constructor stub

}

VelosoChurnModel::~VelosoChurnModel() {
    // TODO Auto-generated destructor stub
}

//void VelosoChurnModel::initialize(int stage)
void VelosoChurnModel::initialize()
{
    sig_arrivalTime = registerSignal("arrivalTime");
    sig_sessionDuration = registerSignal("sessionDuration");

    // get a pointer to the NotificationBoard module and IInterfaceTable
    nb = NotificationBoardAccess().get();

    nb->subscribe(this, NF_INTERFACE_CREATED);
    nb->subscribe(this, NF_INTERFACE_DELETED);
    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

    // -- Get parameters
    param_rng       = par("rng");
    param_a         = par("a");
    param_b         = par("b");
    param_mu        = par("mu");
    param_lambda    = par("lambda");
}

void VelosoChurnModel::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

void VelosoChurnModel::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

double VelosoChurnModel::getArrivalTime()
{
    if (param_a <= 0 || param_b <= 0) return -1.0;

    m_absoluteInterval = (m_absoluteInterval < 0.0)?simTime().dbl():m_absoluteInterval;

    // -- Get an interval which follows an exponential distribution with rate param_arrivalRate
    //double deltaT = pareto_shifted(param_a, param_b, 0, param_rng);;
    double deltaT = pareto_shifted(param_a, param_b, 0);

    // -- Accumulate the value into the origine
    m_absoluteInterval += deltaT;

    // -- Emitting signals for statistics collection
    emit(sig_arrivalTime, m_absoluteInterval);

    return m_absoluteInterval;
}

double VelosoChurnModel::getSessionDuration()
{
    double mean = exp(param_mu + 0.5 * param_lambda * param_lambda);
    double variance =  (exp(param_lambda * param_lambda) - 1) * (exp(2 * param_mu + param_lambda * param_lambda));

    if (variance < 0) return -1.0;

    double stddev = sqrt(variance);

    //double duration = lognormal(mean, stddev, param_rng);
    double duration = lognormal(mean, stddev);

    // -- Emitting signals for statistics collection
    emit(sig_sessionDuration, duration);

    return duration;
}
