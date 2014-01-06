#ifndef MTreeBONEINTERFACES_H_
#define MTreeBONEINTERFACES_H_

#include "MTreeBoneBase.h"
#include "csimplemodule.h"
#include "CommBase.h"

class MTreeBoneBase;

class MTreeBoneInterfaceBase : public CommBase{
public:
    MTreeBoneInterfaceBase(){    m_localPort = getLocalPort();
    m_destPort = getDestPort();};
    virtual ~MTreeBoneInterfaceBase(){};

    virtual void doInit(){};
    virtual void joinNetwork(){};
    virtual void leaveNetwork(){};

    virtual void timerCheckNeighbors(){};

    void setBase(MTreeBoneBase* base){m_Base = base;};

    void sendPacket(cPacket *pkt, IPvXAddress dest) {
        sendToDispatcher(pkt, m_localPort, dest, m_destPort);
    }
protected:
    MTreeBoneBase* m_Base;

    int m_localPort, m_destPort;
};


class MTreeBoneMeshInterface : public MTreeBoneInterfaceBase{
public:
    MTreeBoneMeshInterface(){};
    virtual ~MTreeBoneMeshInterface(){};

    virtual void timerBufferMaps() = 0;
    virtual void timerChunkScheduler() = 0;
};

class MTreeBoneTreeInterface : public MTreeBoneInterfaceBase{
public:
    MTreeBoneTreeInterface(){};
    virtual ~MTreeBoneTreeInterface(){};
};



#endif /* MTreeBONEINTERFACES_H_ */
