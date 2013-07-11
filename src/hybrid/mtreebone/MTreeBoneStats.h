#ifndef MTREEBONESTATS_H_
#define MTREEBONESTATS_H_

#include "genericList.h"
#include "csimplemodule.h"

#include "MTreeBoneSource.h"
#include "MTreeBonePeer.h"
#include "AppSettingDonet.h"

class MTreeBoneStats : public cSimpleModule {
public:
    MTreeBoneStats();
    virtual ~MTreeBoneStats();

    void setSource(MTreeBoneSource* src){m_Src = src;}
    void addPeer(MTreeBonePeer* peer){m_Peers.addItem(peer);}
    void removePeer(MTreeBonePeer* peer){m_Peers.removeItem(peer);}

    static MTreeBoneStats* theStats; // global pointer for easier acces

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    void handleMessage(cMessage *msg);
protected:
    MTreeBoneSource* m_Src;
    genericList<MTreeBonePeer*> m_Peers;
    int m_Stripes;

    cMessage* m_Timer_Report;

    void doReportForStripe(int stripe);
};

#endif /* MTREEBONESTATS_H_ */
