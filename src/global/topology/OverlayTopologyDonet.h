#ifndef OVERLAYTOPOLOGYDONET_H
#define OVERLAYTOPOLOGYDONET_H

#include "TopologyModel.h"
#include "AppSettingDonet.h"
#include "OverlayTopology.h"

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

private:
    AppSettingDonet* m_appSetting;

    std::string param_sourceAddress;
    long m_observedChunk;
    int param_observeTime;

};

#endif // OVERLAYTOPOLOGYDONET_H
