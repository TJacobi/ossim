#include "OverlayTopologyDonet.h"
#include <boost/lexical_cast.hpp>
#include <arpa/inet.h>
#include <assert.h>

using namespace std;
using boost::lexical_cast;

Define_Module(OverlayTopologyDonet);


void OverlayTopologyDonet::initialize()
{
   param_sourceAddress  = par("sourceAddress").stringValue();

   param_observeTime    = par("observeTime").longValue();

   // -- Set pointers to global modules
   cModule *temp = simulation.getModuleByPath("appSetting");
   m_appSetting = check_and_cast<AppSettingDonet *>(temp);
   EV << "Binding to AppSetting was completed!" << endl;

   m_observedChunk  = param_observeTime * m_appSetting->getVideoStreamChunkRate();

   topo[m_observedChunk % INT_MAX] = TopologyModel();

   setRoot(IPvXAddress(param_sourceAddress.c_str()), m_observedChunk % INT_MAX);

   WATCH(param_sourceAddress);
   WATCH(m_observedChunk);
}

void OverlayTopologyDonet::finish() {
}

void OverlayTopologyDonet::handleMessage(cMessage* msg) {
   throw cRuntimeError("OverlayTopology::handleMessage");
}


void OverlayTopologyDonet::collectEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to)
{
   //   if (topo.hasNode())
   addEdge(sequence, from, to);
}
