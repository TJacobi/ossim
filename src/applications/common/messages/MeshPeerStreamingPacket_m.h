//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/common/messages/MeshPeerStreamingPacket.msg.
//

#ifndef _MESHPEERSTREAMINGPACKET_M_H_
#define _MESHPEERSTREAMINGPACKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0402
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include <PeerStreamingPacket_m.h>
// }}



/**
 * Enum generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * enum MeshPeerStreamingPacketType
 * {
 *     MESH_PARTNERSHIP    = 0;
 *     MESH_KEEP_ALIVE     = 1;
 *     MESH_BUFFER_MAP     = 2;
 *     MESH_CHUNK_REQUEST  = 3;
 *     MESH_CHUNK_REPLY    = 4;
 *     MESH_VIDEO_CHUNK    = 5;
 *     MESH_PARTNERSHIP_REQUEST    = 10;
 *     MESH_PARTNERSHIP_ACCEPT     = 11;
 *     MESH_PARTNERSHIP_REJECT     = 12;
 * }
 * </pre>
 */
enum MeshPeerStreamingPacketType {
    MESH_PARTNERSHIP = 0,
    MESH_KEEP_ALIVE = 1,
    MESH_BUFFER_MAP = 2,
    MESH_CHUNK_REQUEST = 3,
    MESH_CHUNK_REPLY = 4,
    MESH_VIDEO_CHUNK = 5,
    MESH_PARTNERSHIP_REQUEST = 10,
    MESH_PARTNERSHIP_ACCEPT = 11,
    MESH_PARTNERSHIP_REJECT = 12
};

/**
 * Class generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet MeshPeerStreamingPacket extends PeerStreamingPacket
 * {
 *     packetGroup = PACKET_GROUP_MESH_OVERLAY;
 *     short packetType @getter(getPacketType) @setter(setPacketType) @enum(MeshPeerStreamingPacketType) ;
 * }
 * </pre>
 */
class MeshPeerStreamingPacket : public ::PeerStreamingPacket
{
  protected:
    short packetType_var;

  private:
    void copy(const MeshPeerStreamingPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MeshPeerStreamingPacket&);

  public:
    MeshPeerStreamingPacket(const char *name=NULL, int kind=0);
    MeshPeerStreamingPacket(const MeshPeerStreamingPacket& other);
    virtual ~MeshPeerStreamingPacket();
    MeshPeerStreamingPacket& operator=(const MeshPeerStreamingPacket& other);
    virtual MeshPeerStreamingPacket *dup() const {return new MeshPeerStreamingPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual short getPacketType() const;
    virtual void setPacketType(short packetType);
};

inline void doPacking(cCommBuffer *b, MeshPeerStreamingPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MeshPeerStreamingPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet MeshPartnershipRequestPacket extends MeshPeerStreamingPacket
 * {
 *     packetType = MESH_PARTNERSHIP_REQUEST;
 *     double upBw @default(-1.0);
 * }
 * </pre>
 */
class MeshPartnershipRequestPacket : public ::MeshPeerStreamingPacket
{
  protected:
    double upBw_var;

  private:
    void copy(const MeshPartnershipRequestPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MeshPartnershipRequestPacket&);

  public:
    MeshPartnershipRequestPacket(const char *name=NULL, int kind=0);
    MeshPartnershipRequestPacket(const MeshPartnershipRequestPacket& other);
    virtual ~MeshPartnershipRequestPacket();
    MeshPartnershipRequestPacket& operator=(const MeshPartnershipRequestPacket& other);
    virtual MeshPartnershipRequestPacket *dup() const {return new MeshPartnershipRequestPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual double getUpBw() const;
    virtual void setUpBw(double upBw);
};

inline void doPacking(cCommBuffer *b, MeshPartnershipRequestPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MeshPartnershipRequestPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet MeshPartnershipAcceptPacket extends MeshPeerStreamingPacket
 * {
 *     packetType = MESH_PARTNERSHIP_ACCEPT;
 *     double upBw @default(-1.0);
 * }
 * </pre>
 */
class MeshPartnershipAcceptPacket : public ::MeshPeerStreamingPacket
{
  protected:
    double upBw_var;

  private:
    void copy(const MeshPartnershipAcceptPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MeshPartnershipAcceptPacket&);

  public:
    MeshPartnershipAcceptPacket(const char *name=NULL, int kind=0);
    MeshPartnershipAcceptPacket(const MeshPartnershipAcceptPacket& other);
    virtual ~MeshPartnershipAcceptPacket();
    MeshPartnershipAcceptPacket& operator=(const MeshPartnershipAcceptPacket& other);
    virtual MeshPartnershipAcceptPacket *dup() const {return new MeshPartnershipAcceptPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual double getUpBw() const;
    virtual void setUpBw(double upBw);
};

inline void doPacking(cCommBuffer *b, MeshPartnershipAcceptPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MeshPartnershipAcceptPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet MeshPartnershipRejectPacket extends MeshPeerStreamingPacket
 * {
 *     packetType = MESH_PARTNERSHIP_REJECT;
 * }
 * </pre>
 */
class MeshPartnershipRejectPacket : public ::MeshPeerStreamingPacket
{
  protected:

  private:
    void copy(const MeshPartnershipRejectPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MeshPartnershipRejectPacket&);

  public:
    MeshPartnershipRejectPacket(const char *name=NULL, int kind=0);
    MeshPartnershipRejectPacket(const MeshPartnershipRejectPacket& other);
    virtual ~MeshPartnershipRejectPacket();
    MeshPartnershipRejectPacket& operator=(const MeshPartnershipRejectPacket& other);
    virtual MeshPartnershipRejectPacket *dup() const {return new MeshPartnershipRejectPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, MeshPartnershipRejectPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MeshPartnershipRejectPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet MeshBufferMapPacket extends MeshPeerStreamingPacket
 * {
 *     packetType = MESH_BUFFER_MAP;
 *     
 *     
 *     
 *     long bmStart_seqNum     @getter(getBmStartSeqNum)   @setter(setBmStartSeqNum);
 *     long bmEnd_seqNum       @getter(getBmEndSeqNum)     @setter(setBmEndSeqNum);
 *     long head_seqNum        @getter(getHeadSeqNum)      @setter(setHeadSeqNum);
 *     
 *     bool bufferMap[];
 * }
 * </pre>
 */
class MeshBufferMapPacket : public ::MeshPeerStreamingPacket
{
  protected:
    long bmStart_seqNum_var;
    long bmEnd_seqNum_var;
    long head_seqNum_var;
    bool *bufferMap_var; // array ptr
    unsigned int bufferMap_arraysize;

  private:
    void copy(const MeshBufferMapPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MeshBufferMapPacket&);

  public:
    MeshBufferMapPacket(const char *name=NULL, int kind=0);
    MeshBufferMapPacket(const MeshBufferMapPacket& other);
    virtual ~MeshBufferMapPacket();
    MeshBufferMapPacket& operator=(const MeshBufferMapPacket& other);
    virtual MeshBufferMapPacket *dup() const {return new MeshBufferMapPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual long getBmStartSeqNum() const;
    virtual void setBmStartSeqNum(long bmStart_seqNum);
    virtual long getBmEndSeqNum() const;
    virtual void setBmEndSeqNum(long bmEnd_seqNum);
    virtual long getHeadSeqNum() const;
    virtual void setHeadSeqNum(long head_seqNum);
    virtual void setBufferMapArraySize(unsigned int size);
    virtual unsigned int getBufferMapArraySize() const;
    virtual bool getBufferMap(unsigned int k) const;
    virtual void setBufferMap(unsigned int k, bool bufferMap);
};

inline void doPacking(cCommBuffer *b, MeshBufferMapPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MeshBufferMapPacket& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>applications/common/messages/MeshPeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet MeshChunkRequestPacket extends MeshPeerStreamingPacket
 * {
 *     packetType = MESH_CHUNK_REQUEST;
 *     
 *     long seqNum_mapStart     @getter(getSeqNumMapStart)     @setter(setSeqNumMapStart);
 *     long seqNum_mapEnd       @getter(getSeqNumMapEnd)       @setter(setSeqNumMapEnd);
 *     long seqNum_mapHead      @getter(getSeqNumMapHead)      @setter(setSeqNumMapHead);
 *     
 *     bool requestMap[];
 * }
 * </pre>
 */
class MeshChunkRequestPacket : public ::MeshPeerStreamingPacket
{
  protected:
    long seqNum_mapStart_var;
    long seqNum_mapEnd_var;
    long seqNum_mapHead_var;
    bool *requestMap_var; // array ptr
    unsigned int requestMap_arraysize;

  private:
    void copy(const MeshChunkRequestPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MeshChunkRequestPacket&);

  public:
    MeshChunkRequestPacket(const char *name=NULL, int kind=0);
    MeshChunkRequestPacket(const MeshChunkRequestPacket& other);
    virtual ~MeshChunkRequestPacket();
    MeshChunkRequestPacket& operator=(const MeshChunkRequestPacket& other);
    virtual MeshChunkRequestPacket *dup() const {return new MeshChunkRequestPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual long getSeqNumMapStart() const;
    virtual void setSeqNumMapStart(long seqNum_mapStart);
    virtual long getSeqNumMapEnd() const;
    virtual void setSeqNumMapEnd(long seqNum_mapEnd);
    virtual long getSeqNumMapHead() const;
    virtual void setSeqNumMapHead(long seqNum_mapHead);
    virtual void setRequestMapArraySize(unsigned int size);
    virtual unsigned int getRequestMapArraySize() const;
    virtual bool getRequestMap(unsigned int k) const;
    virtual void setRequestMap(unsigned int k, bool requestMap);
};

inline void doPacking(cCommBuffer *b, MeshChunkRequestPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MeshChunkRequestPacket& obj) {obj.parsimUnpack(b);}


#endif // _MESHPEERSTREAMINGPACKET_M_H_
