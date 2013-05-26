#ifndef OVERLAYTOPOLOGYDONET_H
#define OVERLAYTOPOLOGYDONET_H

#include "TopologyModel.h"
#include "AppSettingDonet.h"
#include "OverlayTopology.h"

#include <fstream>

class OverlayTopologyDonet : public OverlayTopology
{
protected:
   void initialize();
   //virtual int numInitStages() const { return 5; }
   void finish();
   void handleMessage(cMessage* msg);

public:
   // -- Giang
   inline long getObservedChunk() { return m_observedChunk; }
   void collectEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to);

   void addNode(const IPvXAddress& ip, const int sequence);  ///< for hierarchy per sequence
   void addEdge(const int sequence, const IPvXAddress& from, const IPvXAddress& to); ///< for hierarchy per sequence

//   virtual TopologyModel getTopology(int sequence);
   TopologyModel getTopology(int sequence);

private:
   AppSettingDonet* m_appSetting;

   std::string param_sourceAddress;
   long m_observedChunk;
   int param_observeTime;

   // -- result recording

   std::ofstream m_outFile;

};

#endif // OVERLAYTOPOLOGYDONET_H
