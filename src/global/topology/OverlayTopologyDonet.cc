#include "OverlayTopologyDonet.h"
#include <boost/lexical_cast.hpp>
//#include <arpa/inet.h>
//#include <assert.h>

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

   //m_outFile.open(m_outputFileName.c_str(), fstream::out);
   m_outFile.open(par("outputFilename").stringValue(), fstream::out);

   WATCH(param_sourceAddress);
   WATCH(m_observedChunk);
}

void OverlayTopologyDonet::finish()
{
//   m_outFile << "test" << endl;
//   m_outFile << "m_observedChunk: " << m_observedChunk << endl;
//   m_outFile << "getnumVerticesSequence: " << getNumVerticesSequence(m_observedChunk % INT_MAX) << endl;

   std::list<PPEdge> edgeList = getEdges(m_observedChunk % INT_MAX);

   for(std::list<PPEdge>::iterator iter = edgeList.begin(); iter != edgeList.end(); ++iter)
   {
      m_outFile << iter->begin() << ";" << iter->end() << endl;
   }

   m_outFile.close();
}

void OverlayTopologyDonet::handleMessage(cMessage* msg) {
   throw cRuntimeError("OverlayTopology::handleMessage");
}


void OverlayTopologyDonet::collectEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to)
{
   //   if (topo.hasNode())
   addEdge(sequence, from, to);
}

void OverlayTopologyDonet::addNode(const IPvXAddress& ip, const int sequence)
{
//   m_outFile << ip.str() << "sequence: " << sequence << endl;
   OverlayTopology::addNode(ip, sequence);
}

void OverlayTopologyDonet::addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to)
{
//   m_outFile << "sequence: " << sequence << " -- " << from.str() << " - " << to.str() << endl;
   OverlayTopology::addEdge(sequence, from, to);
}

TopologyModel OverlayTopologyDonet::getTopology(int sequence)
{
   return topo[sequence];
}
