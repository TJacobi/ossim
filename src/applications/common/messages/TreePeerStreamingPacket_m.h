//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/common/messages/TreePeerStreamingPacket.msg.
//

#ifndef _TREEPEERSTREAMINGPACKET_M_H_
#define _TREEPEERSTREAMINGPACKET_M_H_

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
 * Enum generated from <tt>applications/common/messages/TreePeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * enum TreePeerStreamingPacketType
 * {
 *     TREE_KEEP_ALIVE     = 1;
 * }
 * </pre>
 */
enum TreePeerStreamingPacketType {
    TREE_KEEP_ALIVE = 1
};

/**
 * Class generated from <tt>applications/common/messages/TreePeerStreamingPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet TreePeerStreamingPacket extends PeerStreamingPacket
 * {
 *     packetGroup = PACKET_GROUP_TREE_OVERLAY;
 *     short packetType @getter(getPacketType) @setter(setPacketType) @enum(TreePeerStreamingPacketType) ;
 * }
 * </pre>
 */
class TreePeerStreamingPacket : public ::PeerStreamingPacket
{
  protected:
    short packetType_var;

  private:
    void copy(const TreePeerStreamingPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const TreePeerStreamingPacket&);

  public:
    TreePeerStreamingPacket(const char *name=NULL, int kind=0);
    TreePeerStreamingPacket(const TreePeerStreamingPacket& other);
    virtual ~TreePeerStreamingPacket();
    TreePeerStreamingPacket& operator=(const TreePeerStreamingPacket& other);
    virtual TreePeerStreamingPacket *dup() const {return new TreePeerStreamingPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual short getPacketType() const;
    virtual void setPacketType(short packetType);
};

inline void doPacking(cCommBuffer *b, TreePeerStreamingPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, TreePeerStreamingPacket& obj) {obj.parsimUnpack(b);}


#endif // _TREEPEERSTREAMINGPACKET_M_H_
