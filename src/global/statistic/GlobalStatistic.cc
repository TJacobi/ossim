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

#include "GlobalStatistic.h"

Define_Module(GlobalStatistic);
//Define_Module(ActivePeerTable);


GlobalStatistic::GlobalStatistic() {
    // TODO Auto-generated constructor stub

}

GlobalStatistic::~GlobalStatistic() {
    // TODO Auto-generated destructor stub
}

//void GlobalStatistic::initialize()
void GlobalStatistic::initialize(int stage)
{
    if (stage == 0)
    {
        // -- Performance-critical Signals
        // sig_dummy_chunkHit = registerSignal("Signal_ChunkHit");

        sig_chunkHit    = registerSignal("Signal_ChunkHit");
        sig_chunkMiss   = registerSignal("Signal_ChunkMiss");
        sig_chunkSeek   = registerSignal("Signal_ChunkSeek");
        sig_rebuffering   = registerSignal("Signal_Rebuffering");

        sig_meshJoin    = registerSignal("Signal_MeshJoin");
        sig_nPartner    = registerSignal("Signal_NumberOfPartner");

//        std::vector<cIListener*> localListener;
//        localListener = getLocalSignalListeners(sig_dummy_chunkHit);
//
//        for (std::vector<cIListener*>::iterator iter = localListener.begin(); iter != localListener.end(); ++iter)
//        {
//            simulation.getSystemModule()->subscribe("Signal_ChunkHit", *iter);
//        }
    }

    if (stage != 3)
        return;

    // get a pointer to the NotificationBoard module and IInterfaceTable
    nb = NotificationBoardAccess().get();

    nb->subscribe(this, NF_INTERFACE_CREATED);
    nb->subscribe(this, NF_INTERFACE_DELETED);
    nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
    nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
    nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);

//    cNumericResultRecorder *listener = new cNumericResultRecorder;
//    simulation.getSystemModule()->subscribe("chunkHit_Global", listener);

    // -- Initialize count variables
//    m_sizeInView = 0L;
//    m_sizePartialView = 0L;

//    m_countIV = 0L;
//    m_countPV = 0L;

    m_joinTime.setName("JoinTime");

//    m_finalSizeIV.setName("Final Size of InViews");
//    m_finalSizePV.setName("Final Size of PartialViews");

    m_count_NEW = 0L;
    m_count_IGN = 0L;
    m_count_ACK = 0L;

    // -- data to be processed with R
//    m_accumulatedSizePV = 0L;
//    m_totalNode = 0;

    // -- For collecting the sizes of Partial View
    // m_sizePV.setName("pvSize");

    // -- For drawing the histogram of partial view sizes
    m_allFinalPVsizes.setName("allFinalPVsizes");

    // -- For checking the number of new App messages created and deleted
    m_countAppMsgNew = 0L;
    m_countSelfAppMsg = 0L;

    m_countReach = 0L;
}

void GlobalStatistic::handleMessage(cMessage *)
{
    EV << "ActivePeerTable doesn't process messages!" << endl;
}

void GlobalStatistic::receiveChangeNotification(int category, const cPolymorphic *details)
{
    return;
}

void GlobalStatistic::finish()
{
//    EV << "Everage size of InView: " << getAverageSizeInView() << endl;
//    EV << "Everage size of PartialView: " << getAverageSizePartialView() << endl;

    // -- Final Size of InViews & PartialViews
//    m_finalSizeIV.record();
//    m_finalSizePV.record();

//    EV << "InView statistics: Max: " << m_finalSizeIV.getMax()
//            << " -- Min: " << m_finalSizeIV.getMin()
//            << " -- Mean: " << m_finalSizeIV.getMean() << endl;

//    EV << "PartialView statistics: Max: " << m_finalSizePV.getMax()
//                << " -- Min: " << m_finalSizePV.getMin()
//                << " -- Mean: " << m_finalSizePV.getMean() << endl;

    EV << "Work around with subscriptions: " << endl;
    EV << "NEW: " << m_count_NEW << " -- IGN: " << m_count_IGN << " -- ACK: " << m_count_ACK << endl;

    // -- Record a scalar value
//    EV << "Accumulated PVsize: " << m_accumulatedSizePV << endl;
//    EV << "Number of reported nodes: " << m_totalNode << endl;
//    recordScalar("Average PartialView Size", m_accumulatedSizePV/(double)m_totalNode);

    // -- For the histogram of the partial view sizes
//    m_sizePV.recordAs("histogram of pvSize");

    // -- For the histogram of the partial view sizes
//    m_sizeIV.recordAs("histogram of ivSize");

    // -- For checking the number of new App messages created and deleted
    EV << "The number of App message left: " << m_countAppMsgNew << endl;
    EV << "The number of ignored messaged (because of returning to sender: " << m_countSelfAppMsg << endl;

    // -- For calculating and recording reach ratio
    EV << "The reach ratio (it should approximate the total number of nodes): "
            << 1 + (double)m_countReach / m_countAppMsgNew << endl;

    recordScalar("Reach Ratio", (double)m_countReach / m_countAppMsgNew);
}

// ----------------- Interface in effect -------------------------

/**
 * ****************************************************************************
 * Recording the sizes of InView & PartialView
 * ****************************************************************************
 */

//void GlobalStatistic::recordSizeInView(int size)
//{
//    m_sizeInView += size;
//    ++m_countIV;
//}

//void GlobalStatistic::recordSizePartialView(int size)
//{
//    m_sizePartialView += size;
//    ++m_countPV;
//}

//double GlobalStatistic::getAverageSizeInView(void)
//{
//    return (double)m_sizeInView / m_countIV;
//}

//double GlobalStatistic::getAverageSizePartialView(void)
//{
//    return (double)m_sizePartialView / m_countPV;
//}



void GlobalStatistic::recordJoinTime(double time)
{
    m_joinTime.record(time);
}

/**
 * ****************************************************************************
 * Collect sizes of InView & PartialView
 * ****************************************************************************
 */

void GlobalStatistic::collectSizeIV(int size)
{
//    m_finalSizeIV.collect(size);
}

void GlobalStatistic::collectSizePV(int size)
{
//    m_finalSizePV.collect(size);
}

/**
 * ****************************************************************************
 * Work-around with counting the subscriptions
 * ****************************************************************************
 */

void GlobalStatistic::increaseNEW(void)
{
    ++m_count_NEW;
}

void GlobalStatistic::decreaseNEW(void)
{
    --m_count_NEW;
}

void GlobalStatistic::increaseIGN(void)
{
    ++m_count_IGN;
}

void GlobalStatistic::increaseACK(void)
{
    ++m_count_ACK;
}

long GlobalStatistic::getNEW(void)
{
    return m_count_NEW;
}

long GlobalStatistic::getIGN(void)
{
    return m_count_IGN;
}

long GlobalStatistic::getACK(void)
{
    return m_count_ACK;
}

//void GlobalStatistic::recordPartialViewSize(int size)
//{
//    m_accumulatedSizePV += (long)size;
//    ++m_totalNode;
//}

//void GlobalStatistic::collectPartialViewSize(int size)
//{
//    m_sizePV.collect(size);
//}

//void GlobalStatistic::collectInViewSize(int size)
//{
//    m_sizeIV.collect(size);
//}

void GlobalStatistic::collectAllPVsizes(int size)
{
    m_allFinalPVsizes.record(size);
}

// ---------------------- Signal --------------------------
//void GlobalStatistic::receiveSignal(cComponent *src, simsignal_t id, long l){
//        if(registerSignal("BeaconTx") == id){
//                int idx = (int) l ;
//                txBeacons[idx]++ ;
//        }
//        else if(registerSignal("AppTx") == id){
//                appTx++ ;
//        }
//        else opp_error("Unknown signal received. 2") ;
//}
//
//void GlobalStatistic::receiveSignal(cComponent *src, simsignal_t id, cObject* obj){
//
//        if(registerSignal("BeaconRx") == id){
//                beaconRx* brx = check_and_cast<beaconRx*>(obj) ;
//                rxBeacons[index(brx->id,brx->from,totNodes)]++ ;
//        }
//        else opp_error("Unknown signal.") ;
//}

//void GlobalStatistic::reportChunkHit(SEQUENCE_NUMBER_T seq_num)
void GlobalStatistic::reportChunkHit(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_chunkHit, seq_num);
}

void GlobalStatistic::reportChunkMiss(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_chunkMiss, seq_num);
}

void GlobalStatistic::reportChunkSeek(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_chunkSeek, seq_num);
}

void GlobalStatistic::reportRebuffering(const SEQUENCE_NUMBER_T &seq_num)
{
    emit(sig_rebuffering, seq_num);
}

void GlobalStatistic::reportMeshJoin()
{
    emit(sig_meshJoin, 1);
}

void GlobalStatistic::reportNumberOfPartner(int nPartner)
{
    emit(sig_nPartner, nPartner);
}
