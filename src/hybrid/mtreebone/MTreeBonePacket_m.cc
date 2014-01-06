//
// Generated file, do not edit! Created by opp_msgc 4.2 from hybrid/mtreebone/MTreeBonePacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "MTreeBonePacket_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("MTreeBonePacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("MTreeBonePacketType"));
    e->insert(MTREEBONE_BUFFER_MAP, "MTREEBONE_BUFFER_MAP");
    e->insert(MTREEBONE_NEIGHBOR_REQUEST, "MTREEBONE_NEIGHBOR_REQUEST");
    e->insert(MTREEBONE_NEIGHBOR_REQUEST_RESPONSE, "MTREEBONE_NEIGHBOR_REQUEST_RESPONSE");
    e->insert(MTREEBONE_PARENT_REQUEST, "MTREEBONE_PARENT_REQUEST");
    e->insert(MTREEBONE_PARENT_REQUEST_RESPONSE, "MTREEBONE_PARENT_REQUEST_RESPONSE");
    e->insert(MTREEBONE_CHUNK_REQUEST_LIST, "MTREEBONE_CHUNK_REQUEST_LIST");
    e->insert(MTREEBONE_PEER_INFORMATION, "MTREEBONE_PEER_INFORMATION");
    e->insert(MTREEBONE_SWITCH_POSITION_REQUEST, "MTREEBONE_SWITCH_POSITION_REQUEST");
    e->insert(MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE, "MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE");
    e->insert(MTREEBONE_REPLACE_CHILD, "MTREEBONE_REPLACE_CHILD");
    e->insert(MTREEBONE_INFORM_NEW_PARENT, "MTREEBONE_INFORM_NEW_PARENT");
    e->insert(MTREEBONE_PROMOTION, "MTREEBONE_PROMOTION");
    e->insert(MTREEBONE_PACKETS_MAX, "MTREEBONE_PACKETS_MAX");
);

Register_Class(MTreeBonePacket);

MTreeBonePacket::MTreeBonePacket(const char *name, int kind) : PeerStreamingPacket(name,kind)
{
    this->packetType_var = 0;
}

MTreeBonePacket::MTreeBonePacket(const MTreeBonePacket& other) : PeerStreamingPacket(other)
{
    copy(other);
}

MTreeBonePacket::~MTreeBonePacket()
{
}

MTreeBonePacket& MTreeBonePacket::operator=(const MTreeBonePacket& other)
{
    if (this==&other) return *this;
    PeerStreamingPacket::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePacket::copy(const MTreeBonePacket& other)
{
    this->packetType_var = other.packetType_var;
}

void MTreeBonePacket::parsimPack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimPack(b);
    doPacking(b,this->packetType_var);
}

void MTreeBonePacket::parsimUnpack(cCommBuffer *b)
{
    PeerStreamingPacket::parsimUnpack(b);
    doUnpacking(b,this->packetType_var);
}

short MTreeBonePacket::getPacketType() const
{
    return packetType_var;
}

void MTreeBonePacket::setPacketType(short packetType)
{
    this->packetType_var = packetType;
}

class MTreeBonePacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePacketDescriptor();
    virtual ~MTreeBonePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePacketDescriptor);

MTreeBonePacketDescriptor::MTreeBonePacketDescriptor() : cClassDescriptor("MTreeBonePacket", "PeerStreamingPacket")
{
}

MTreeBonePacketDescriptor::~MTreeBonePacketDescriptor()
{
}

bool MTreeBonePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePacket *>(obj)!=NULL;
}

const char *MTreeBonePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int MTreeBonePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBonePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "packetType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int MTreeBonePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBonePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"getter")) return "getPacketType";
            if (!strcmp(propertyname,"enum")) return "MTreeBonePacketType";
            if (!strcmp(propertyname,"setter")) return "setPacketType";
            return NULL;
        default: return NULL;
    }
}

int MTreeBonePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacket *pp = (MTreeBonePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacket *pp = (MTreeBonePacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPacketType());
        default: return "";
    }
}

bool MTreeBonePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacket *pp = (MTreeBonePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType(string2long(value)); return true;
        default: return false;
    }
}

const char *MTreeBonePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *MTreeBonePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacket *pp = (MTreeBonePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBonePacketMesh);

MTreeBonePacketMesh::MTreeBonePacketMesh(const char *name, int kind) : MTreeBonePacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_MESH_OVERLAY);
}

MTreeBonePacketMesh::MTreeBonePacketMesh(const MTreeBonePacketMesh& other) : MTreeBonePacket(other)
{
    copy(other);
}

MTreeBonePacketMesh::~MTreeBonePacketMesh()
{
}

MTreeBonePacketMesh& MTreeBonePacketMesh::operator=(const MTreeBonePacketMesh& other)
{
    if (this==&other) return *this;
    MTreeBonePacket::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePacketMesh::copy(const MTreeBonePacketMesh& other)
{
}

void MTreeBonePacketMesh::parsimPack(cCommBuffer *b)
{
    MTreeBonePacket::parsimPack(b);
}

void MTreeBonePacketMesh::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacket::parsimUnpack(b);
}

class MTreeBonePacketMeshDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePacketMeshDescriptor();
    virtual ~MTreeBonePacketMeshDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePacketMeshDescriptor);

MTreeBonePacketMeshDescriptor::MTreeBonePacketMeshDescriptor() : cClassDescriptor("MTreeBonePacketMesh", "MTreeBonePacket")
{
}

MTreeBonePacketMeshDescriptor::~MTreeBonePacketMeshDescriptor()
{
}

bool MTreeBonePacketMeshDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePacketMesh *>(obj)!=NULL;
}

const char *MTreeBonePacketMeshDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePacketMeshDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int MTreeBonePacketMeshDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *MTreeBonePacketMeshDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int MTreeBonePacketMeshDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePacketMeshDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *MTreeBonePacketMeshDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePacketMeshDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketMesh *pp = (MTreeBonePacketMesh *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePacketMeshDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketMesh *pp = (MTreeBonePacketMesh *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool MTreeBonePacketMeshDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketMesh *pp = (MTreeBonePacketMesh *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *MTreeBonePacketMeshDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *MTreeBonePacketMeshDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketMesh *pp = (MTreeBonePacketMesh *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBonePacketTree);

MTreeBonePacketTree::MTreeBonePacketTree(const char *name, int kind) : MTreeBonePacket(name,kind)
{
    this->setPacketGroup(PACKET_GROUP_TREE_OVERLAY);
}

MTreeBonePacketTree::MTreeBonePacketTree(const MTreeBonePacketTree& other) : MTreeBonePacket(other)
{
    copy(other);
}

MTreeBonePacketTree::~MTreeBonePacketTree()
{
}

MTreeBonePacketTree& MTreeBonePacketTree::operator=(const MTreeBonePacketTree& other)
{
    if (this==&other) return *this;
    MTreeBonePacket::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePacketTree::copy(const MTreeBonePacketTree& other)
{
}

void MTreeBonePacketTree::parsimPack(cCommBuffer *b)
{
    MTreeBonePacket::parsimPack(b);
}

void MTreeBonePacketTree::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacket::parsimUnpack(b);
}

class MTreeBonePacketTreeDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePacketTreeDescriptor();
    virtual ~MTreeBonePacketTreeDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePacketTreeDescriptor);

MTreeBonePacketTreeDescriptor::MTreeBonePacketTreeDescriptor() : cClassDescriptor("MTreeBonePacketTree", "MTreeBonePacket")
{
}

MTreeBonePacketTreeDescriptor::~MTreeBonePacketTreeDescriptor()
{
}

bool MTreeBonePacketTreeDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePacketTree *>(obj)!=NULL;
}

const char *MTreeBonePacketTreeDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePacketTreeDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int MTreeBonePacketTreeDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *MTreeBonePacketTreeDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int MTreeBonePacketTreeDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePacketTreeDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *MTreeBonePacketTreeDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePacketTreeDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketTree *pp = (MTreeBonePacketTree *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePacketTreeDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketTree *pp = (MTreeBonePacketTree *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool MTreeBonePacketTreeDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketTree *pp = (MTreeBonePacketTree *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *MTreeBonePacketTreeDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *MTreeBonePacketTreeDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePacketTree *pp = (MTreeBonePacketTree *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBoneBufferMapPacket);

MTreeBoneBufferMapPacket::MTreeBoneBufferMapPacket(const char *name, int kind) : MTreeBonePacketMesh(name,kind)
{
    this->setPacketType(MTREEBONE_BUFFER_MAP);

    this->sequenceNumberStart_var = 0;
    this->sequenceNumberEnd_var = 0;
    this->missingChunks_var = 0;
    boneNodeForStripe_arraysize = 0;
    this->boneNodeForStripe_var = 0;
    bufferMap_arraysize = 0;
    this->bufferMap_var = 0;
    this->tempYouAreMyParent_var = 0;
}

MTreeBoneBufferMapPacket::MTreeBoneBufferMapPacket(const MTreeBoneBufferMapPacket& other) : MTreeBonePacketMesh(other)
{
    boneNodeForStripe_arraysize = 0;
    this->boneNodeForStripe_var = 0;
    bufferMap_arraysize = 0;
    this->bufferMap_var = 0;
    copy(other);
}

MTreeBoneBufferMapPacket::~MTreeBoneBufferMapPacket()
{
    delete [] boneNodeForStripe_var;
    delete [] bufferMap_var;
}

MTreeBoneBufferMapPacket& MTreeBoneBufferMapPacket::operator=(const MTreeBoneBufferMapPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketMesh::operator=(other);
    copy(other);
    return *this;
}

void MTreeBoneBufferMapPacket::copy(const MTreeBoneBufferMapPacket& other)
{
    this->sequenceNumberStart_var = other.sequenceNumberStart_var;
    this->sequenceNumberEnd_var = other.sequenceNumberEnd_var;
    this->missingChunks_var = other.missingChunks_var;
    delete [] this->boneNodeForStripe_var;
    this->boneNodeForStripe_var = (other.boneNodeForStripe_arraysize==0) ? NULL : new bool[other.boneNodeForStripe_arraysize];
    boneNodeForStripe_arraysize = other.boneNodeForStripe_arraysize;
    for (unsigned int i=0; i<boneNodeForStripe_arraysize; i++)
        this->boneNodeForStripe_var[i] = other.boneNodeForStripe_var[i];
    delete [] this->bufferMap_var;
    this->bufferMap_var = (other.bufferMap_arraysize==0) ? NULL : new bool[other.bufferMap_arraysize];
    bufferMap_arraysize = other.bufferMap_arraysize;
    for (unsigned int i=0; i<bufferMap_arraysize; i++)
        this->bufferMap_var[i] = other.bufferMap_var[i];
    this->tempYouAreMyParent_var = other.tempYouAreMyParent_var;
}

void MTreeBoneBufferMapPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimPack(b);
    doPacking(b,this->sequenceNumberStart_var);
    doPacking(b,this->sequenceNumberEnd_var);
    doPacking(b,this->missingChunks_var);
    b->pack(boneNodeForStripe_arraysize);
    doPacking(b,this->boneNodeForStripe_var,boneNodeForStripe_arraysize);
    b->pack(bufferMap_arraysize);
    doPacking(b,this->bufferMap_var,bufferMap_arraysize);
    doPacking(b,this->tempYouAreMyParent_var);
}

void MTreeBoneBufferMapPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimUnpack(b);
    doUnpacking(b,this->sequenceNumberStart_var);
    doUnpacking(b,this->sequenceNumberEnd_var);
    doUnpacking(b,this->missingChunks_var);
    delete [] this->boneNodeForStripe_var;
    b->unpack(boneNodeForStripe_arraysize);
    if (boneNodeForStripe_arraysize==0) {
        this->boneNodeForStripe_var = 0;
    } else {
        this->boneNodeForStripe_var = new bool[boneNodeForStripe_arraysize];
        doUnpacking(b,this->boneNodeForStripe_var,boneNodeForStripe_arraysize);
    }
    delete [] this->bufferMap_var;
    b->unpack(bufferMap_arraysize);
    if (bufferMap_arraysize==0) {
        this->bufferMap_var = 0;
    } else {
        this->bufferMap_var = new bool[bufferMap_arraysize];
        doUnpacking(b,this->bufferMap_var,bufferMap_arraysize);
    }
    doUnpacking(b,this->tempYouAreMyParent_var);
}

unsigned int MTreeBoneBufferMapPacket::getSequenceNumberStart() const
{
    return sequenceNumberStart_var;
}

void MTreeBoneBufferMapPacket::setSequenceNumberStart(unsigned int sequenceNumberStart)
{
    this->sequenceNumberStart_var = sequenceNumberStart;
}

unsigned int MTreeBoneBufferMapPacket::getSequenceNumberEnd() const
{
    return sequenceNumberEnd_var;
}

void MTreeBoneBufferMapPacket::setSequenceNumberEnd(unsigned int sequenceNumberEnd)
{
    this->sequenceNumberEnd_var = sequenceNumberEnd;
}

unsigned int MTreeBoneBufferMapPacket::getMissingChunks() const
{
    return missingChunks_var;
}

void MTreeBoneBufferMapPacket::setMissingChunks(unsigned int missingChunks)
{
    this->missingChunks_var = missingChunks;
}

void MTreeBoneBufferMapPacket::setBoneNodeForStripeArraySize(unsigned int size)
{
    bool *boneNodeForStripe_var2 = (size==0) ? NULL : new bool[size];
    unsigned int sz = boneNodeForStripe_arraysize < size ? boneNodeForStripe_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        boneNodeForStripe_var2[i] = this->boneNodeForStripe_var[i];
    for (unsigned int i=sz; i<size; i++)
        boneNodeForStripe_var2[i] = 0;
    boneNodeForStripe_arraysize = size;
    delete [] this->boneNodeForStripe_var;
    this->boneNodeForStripe_var = boneNodeForStripe_var2;
}

unsigned int MTreeBoneBufferMapPacket::getBoneNodeForStripeArraySize() const
{
    return boneNodeForStripe_arraysize;
}

bool MTreeBoneBufferMapPacket::getBoneNodeForStripe(unsigned int k) const
{
    if (k>=boneNodeForStripe_arraysize) throw cRuntimeError("Array of size %d indexed by %d", boneNodeForStripe_arraysize, k);
    return boneNodeForStripe_var[k];
}

void MTreeBoneBufferMapPacket::setBoneNodeForStripe(unsigned int k, bool boneNodeForStripe)
{
    if (k>=boneNodeForStripe_arraysize) throw cRuntimeError("Array of size %d indexed by %d", boneNodeForStripe_arraysize, k);
    this->boneNodeForStripe_var[k] = boneNodeForStripe;
}

void MTreeBoneBufferMapPacket::setBufferMapArraySize(unsigned int size)
{
    bool *bufferMap_var2 = (size==0) ? NULL : new bool[size];
    unsigned int sz = bufferMap_arraysize < size ? bufferMap_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        bufferMap_var2[i] = this->bufferMap_var[i];
    for (unsigned int i=sz; i<size; i++)
        bufferMap_var2[i] = 0;
    bufferMap_arraysize = size;
    delete [] this->bufferMap_var;
    this->bufferMap_var = bufferMap_var2;
}

unsigned int MTreeBoneBufferMapPacket::getBufferMapArraySize() const
{
    return bufferMap_arraysize;
}

bool MTreeBoneBufferMapPacket::getBufferMap(unsigned int k) const
{
    if (k>=bufferMap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", bufferMap_arraysize, k);
    return bufferMap_var[k];
}

void MTreeBoneBufferMapPacket::setBufferMap(unsigned int k, bool bufferMap)
{
    if (k>=bufferMap_arraysize) throw cRuntimeError("Array of size %d indexed by %d", bufferMap_arraysize, k);
    this->bufferMap_var[k] = bufferMap;
}

bool MTreeBoneBufferMapPacket::getTempYouAreMyParent() const
{
    return tempYouAreMyParent_var;
}

void MTreeBoneBufferMapPacket::setTempYouAreMyParent(bool tempYouAreMyParent)
{
    this->tempYouAreMyParent_var = tempYouAreMyParent;
}

class MTreeBoneBufferMapPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBoneBufferMapPacketDescriptor();
    virtual ~MTreeBoneBufferMapPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBoneBufferMapPacketDescriptor);

MTreeBoneBufferMapPacketDescriptor::MTreeBoneBufferMapPacketDescriptor() : cClassDescriptor("MTreeBoneBufferMapPacket", "MTreeBonePacketMesh")
{
}

MTreeBoneBufferMapPacketDescriptor::~MTreeBoneBufferMapPacketDescriptor()
{
}

bool MTreeBoneBufferMapPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBoneBufferMapPacket *>(obj)!=NULL;
}

const char *MTreeBoneBufferMapPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBoneBufferMapPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int MTreeBoneBufferMapPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBoneBufferMapPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sequenceNumberStart",
        "sequenceNumberEnd",
        "missingChunks",
        "boneNodeForStripe",
        "bufferMap",
        "tempYouAreMyParent",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int MTreeBoneBufferMapPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumberStart")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumberEnd")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "missingChunks")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "boneNodeForStripe")==0) return base+3;
    if (fieldName[0]=='b' && strcmp(fieldName, "bufferMap")==0) return base+4;
    if (fieldName[0]=='t' && strcmp(fieldName, "tempYouAreMyParent")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBoneBufferMapPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBoneBufferMapPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBoneBufferMapPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneBufferMapPacket *pp = (MTreeBoneBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 3: return pp->getBoneNodeForStripeArraySize();
        case 4: return pp->getBufferMapArraySize();
        default: return 0;
    }
}

std::string MTreeBoneBufferMapPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneBufferMapPacket *pp = (MTreeBoneBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSequenceNumberStart());
        case 1: return ulong2string(pp->getSequenceNumberEnd());
        case 2: return ulong2string(pp->getMissingChunks());
        case 3: return bool2string(pp->getBoneNodeForStripe(i));
        case 4: return bool2string(pp->getBufferMap(i));
        case 5: return bool2string(pp->getTempYouAreMyParent());
        default: return "";
    }
}

bool MTreeBoneBufferMapPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneBufferMapPacket *pp = (MTreeBoneBufferMapPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSequenceNumberStart(string2ulong(value)); return true;
        case 1: pp->setSequenceNumberEnd(string2ulong(value)); return true;
        case 2: pp->setMissingChunks(string2ulong(value)); return true;
        case 3: pp->setBoneNodeForStripe(i,string2bool(value)); return true;
        case 4: pp->setBufferMap(i,string2bool(value)); return true;
        case 5: pp->setTempYouAreMyParent(string2bool(value)); return true;
        default: return false;
    }
}

const char *MTreeBoneBufferMapPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
}

void *MTreeBoneBufferMapPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneBufferMapPacket *pp = (MTreeBoneBufferMapPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBoneChunkRequestListPacket);

MTreeBoneChunkRequestListPacket::MTreeBoneChunkRequestListPacket(const char *name, int kind) : MTreeBonePacketMesh(name,kind)
{
    this->setPacketType(MTREEBONE_CHUNK_REQUEST_LIST);

    sequenceNumbers_arraysize = 0;
    this->sequenceNumbers_var = 0;
}

MTreeBoneChunkRequestListPacket::MTreeBoneChunkRequestListPacket(const MTreeBoneChunkRequestListPacket& other) : MTreeBonePacketMesh(other)
{
    sequenceNumbers_arraysize = 0;
    this->sequenceNumbers_var = 0;
    copy(other);
}

MTreeBoneChunkRequestListPacket::~MTreeBoneChunkRequestListPacket()
{
    delete [] sequenceNumbers_var;
}

MTreeBoneChunkRequestListPacket& MTreeBoneChunkRequestListPacket::operator=(const MTreeBoneChunkRequestListPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketMesh::operator=(other);
    copy(other);
    return *this;
}

void MTreeBoneChunkRequestListPacket::copy(const MTreeBoneChunkRequestListPacket& other)
{
    delete [] this->sequenceNumbers_var;
    this->sequenceNumbers_var = (other.sequenceNumbers_arraysize==0) ? NULL : new unsigned int[other.sequenceNumbers_arraysize];
    sequenceNumbers_arraysize = other.sequenceNumbers_arraysize;
    for (unsigned int i=0; i<sequenceNumbers_arraysize; i++)
        this->sequenceNumbers_var[i] = other.sequenceNumbers_var[i];
}

void MTreeBoneChunkRequestListPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimPack(b);
    b->pack(sequenceNumbers_arraysize);
    doPacking(b,this->sequenceNumbers_var,sequenceNumbers_arraysize);
}

void MTreeBoneChunkRequestListPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimUnpack(b);
    delete [] this->sequenceNumbers_var;
    b->unpack(sequenceNumbers_arraysize);
    if (sequenceNumbers_arraysize==0) {
        this->sequenceNumbers_var = 0;
    } else {
        this->sequenceNumbers_var = new unsigned int[sequenceNumbers_arraysize];
        doUnpacking(b,this->sequenceNumbers_var,sequenceNumbers_arraysize);
    }
}

void MTreeBoneChunkRequestListPacket::setSequenceNumbersArraySize(unsigned int size)
{
    unsigned int *sequenceNumbers_var2 = (size==0) ? NULL : new unsigned int[size];
    unsigned int sz = sequenceNumbers_arraysize < size ? sequenceNumbers_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        sequenceNumbers_var2[i] = this->sequenceNumbers_var[i];
    for (unsigned int i=sz; i<size; i++)
        sequenceNumbers_var2[i] = 0;
    sequenceNumbers_arraysize = size;
    delete [] this->sequenceNumbers_var;
    this->sequenceNumbers_var = sequenceNumbers_var2;
}

unsigned int MTreeBoneChunkRequestListPacket::getSequenceNumbersArraySize() const
{
    return sequenceNumbers_arraysize;
}

unsigned int MTreeBoneChunkRequestListPacket::getSequenceNumbers(unsigned int k) const
{
    if (k>=sequenceNumbers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", sequenceNumbers_arraysize, k);
    return sequenceNumbers_var[k];
}

void MTreeBoneChunkRequestListPacket::setSequenceNumbers(unsigned int k, unsigned int sequenceNumbers)
{
    if (k>=sequenceNumbers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", sequenceNumbers_arraysize, k);
    this->sequenceNumbers_var[k] = sequenceNumbers;
}

class MTreeBoneChunkRequestListPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBoneChunkRequestListPacketDescriptor();
    virtual ~MTreeBoneChunkRequestListPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBoneChunkRequestListPacketDescriptor);

MTreeBoneChunkRequestListPacketDescriptor::MTreeBoneChunkRequestListPacketDescriptor() : cClassDescriptor("MTreeBoneChunkRequestListPacket", "MTreeBonePacketMesh")
{
}

MTreeBoneChunkRequestListPacketDescriptor::~MTreeBoneChunkRequestListPacketDescriptor()
{
}

bool MTreeBoneChunkRequestListPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBoneChunkRequestListPacket *>(obj)!=NULL;
}

const char *MTreeBoneChunkRequestListPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBoneChunkRequestListPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int MTreeBoneChunkRequestListPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBoneChunkRequestListPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sequenceNumbers",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int MTreeBoneChunkRequestListPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumbers")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBoneChunkRequestListPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBoneChunkRequestListPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBoneChunkRequestListPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneChunkRequestListPacket *pp = (MTreeBoneChunkRequestListPacket *)object; (void)pp;
    switch (field) {
        case 0: return pp->getSequenceNumbersArraySize();
        default: return 0;
    }
}

std::string MTreeBoneChunkRequestListPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneChunkRequestListPacket *pp = (MTreeBoneChunkRequestListPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getSequenceNumbers(i));
        default: return "";
    }
}

bool MTreeBoneChunkRequestListPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneChunkRequestListPacket *pp = (MTreeBoneChunkRequestListPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setSequenceNumbers(i,string2ulong(value)); return true;
        default: return false;
    }
}

const char *MTreeBoneChunkRequestListPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *MTreeBoneChunkRequestListPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneChunkRequestListPacket *pp = (MTreeBoneChunkRequestListPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBoneNeighborRequestPacket);

MTreeBoneNeighborRequestPacket::MTreeBoneNeighborRequestPacket(const char *name, int kind) : MTreeBonePacketMesh(name,kind)
{
    this->setPacketType(MTREEBONE_NEIGHBOR_REQUEST);

    this->stripeNumber_var = 0;
    this->joinTime_var = 0;
}

MTreeBoneNeighborRequestPacket::MTreeBoneNeighborRequestPacket(const MTreeBoneNeighborRequestPacket& other) : MTreeBonePacketMesh(other)
{
    copy(other);
}

MTreeBoneNeighborRequestPacket::~MTreeBoneNeighborRequestPacket()
{
}

MTreeBoneNeighborRequestPacket& MTreeBoneNeighborRequestPacket::operator=(const MTreeBoneNeighborRequestPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketMesh::operator=(other);
    copy(other);
    return *this;
}

void MTreeBoneNeighborRequestPacket::copy(const MTreeBoneNeighborRequestPacket& other)
{
    this->stripeNumber_var = other.stripeNumber_var;
    this->joinTime_var = other.joinTime_var;
}

void MTreeBoneNeighborRequestPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimPack(b);
    doPacking(b,this->stripeNumber_var);
    doPacking(b,this->joinTime_var);
}

void MTreeBoneNeighborRequestPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimUnpack(b);
    doUnpacking(b,this->stripeNumber_var);
    doUnpacking(b,this->joinTime_var);
}

unsigned int MTreeBoneNeighborRequestPacket::getStripeNumber() const
{
    return stripeNumber_var;
}

void MTreeBoneNeighborRequestPacket::setStripeNumber(unsigned int stripeNumber)
{
    this->stripeNumber_var = stripeNumber;
}

double MTreeBoneNeighborRequestPacket::getJoinTime() const
{
    return joinTime_var;
}

void MTreeBoneNeighborRequestPacket::setJoinTime(double joinTime)
{
    this->joinTime_var = joinTime;
}

class MTreeBoneNeighborRequestPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBoneNeighborRequestPacketDescriptor();
    virtual ~MTreeBoneNeighborRequestPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBoneNeighborRequestPacketDescriptor);

MTreeBoneNeighborRequestPacketDescriptor::MTreeBoneNeighborRequestPacketDescriptor() : cClassDescriptor("MTreeBoneNeighborRequestPacket", "MTreeBonePacketMesh")
{
}

MTreeBoneNeighborRequestPacketDescriptor::~MTreeBoneNeighborRequestPacketDescriptor()
{
}

bool MTreeBoneNeighborRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBoneNeighborRequestPacket *>(obj)!=NULL;
}

const char *MTreeBoneNeighborRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBoneNeighborRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBoneNeighborRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBoneNeighborRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripeNumber",
        "joinTime",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBoneNeighborRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripeNumber")==0) return base+0;
    if (fieldName[0]=='j' && strcmp(fieldName, "joinTime")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBoneNeighborRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "double",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBoneNeighborRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBoneNeighborRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestPacket *pp = (MTreeBoneNeighborRequestPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBoneNeighborRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestPacket *pp = (MTreeBoneNeighborRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getStripeNumber());
        case 1: return double2string(pp->getJoinTime());
        default: return "";
    }
}

bool MTreeBoneNeighborRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestPacket *pp = (MTreeBoneNeighborRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripeNumber(string2ulong(value)); return true;
        case 1: pp->setJoinTime(string2double(value)); return true;
        default: return false;
    }
}

const char *MTreeBoneNeighborRequestPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBoneNeighborRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestPacket *pp = (MTreeBoneNeighborRequestPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBoneNeighborRequestResponsePacket);

MTreeBoneNeighborRequestResponsePacket::MTreeBoneNeighborRequestResponsePacket(const char *name, int kind) : MTreeBonePacketMesh(name,kind)
{
    this->setPacketType(MTREEBONE_NEIGHBOR_REQUEST_RESPONSE);

    this->stripeNumber_var = 0;
    this->isAccepted_var = 0;
}

MTreeBoneNeighborRequestResponsePacket::MTreeBoneNeighborRequestResponsePacket(const MTreeBoneNeighborRequestResponsePacket& other) : MTreeBonePacketMesh(other)
{
    copy(other);
}

MTreeBoneNeighborRequestResponsePacket::~MTreeBoneNeighborRequestResponsePacket()
{
}

MTreeBoneNeighborRequestResponsePacket& MTreeBoneNeighborRequestResponsePacket::operator=(const MTreeBoneNeighborRequestResponsePacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketMesh::operator=(other);
    copy(other);
    return *this;
}

void MTreeBoneNeighborRequestResponsePacket::copy(const MTreeBoneNeighborRequestResponsePacket& other)
{
    this->stripeNumber_var = other.stripeNumber_var;
    this->isAccepted_var = other.isAccepted_var;
}

void MTreeBoneNeighborRequestResponsePacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimPack(b);
    doPacking(b,this->stripeNumber_var);
    doPacking(b,this->isAccepted_var);
}

void MTreeBoneNeighborRequestResponsePacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketMesh::parsimUnpack(b);
    doUnpacking(b,this->stripeNumber_var);
    doUnpacking(b,this->isAccepted_var);
}

unsigned int MTreeBoneNeighborRequestResponsePacket::getStripeNumber() const
{
    return stripeNumber_var;
}

void MTreeBoneNeighborRequestResponsePacket::setStripeNumber(unsigned int stripeNumber)
{
    this->stripeNumber_var = stripeNumber;
}

bool MTreeBoneNeighborRequestResponsePacket::getIsAccepted() const
{
    return isAccepted_var;
}

void MTreeBoneNeighborRequestResponsePacket::setIsAccepted(bool isAccepted)
{
    this->isAccepted_var = isAccepted;
}

class MTreeBoneNeighborRequestResponsePacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBoneNeighborRequestResponsePacketDescriptor();
    virtual ~MTreeBoneNeighborRequestResponsePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBoneNeighborRequestResponsePacketDescriptor);

MTreeBoneNeighborRequestResponsePacketDescriptor::MTreeBoneNeighborRequestResponsePacketDescriptor() : cClassDescriptor("MTreeBoneNeighborRequestResponsePacket", "MTreeBonePacketMesh")
{
}

MTreeBoneNeighborRequestResponsePacketDescriptor::~MTreeBoneNeighborRequestResponsePacketDescriptor()
{
}

bool MTreeBoneNeighborRequestResponsePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBoneNeighborRequestResponsePacket *>(obj)!=NULL;
}

const char *MTreeBoneNeighborRequestResponsePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripeNumber",
        "isAccepted",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBoneNeighborRequestResponsePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripeNumber")==0) return base+0;
    if (fieldName[0]=='i' && strcmp(fieldName, "isAccepted")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "bool",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBoneNeighborRequestResponsePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestResponsePacket *pp = (MTreeBoneNeighborRequestResponsePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestResponsePacket *pp = (MTreeBoneNeighborRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getStripeNumber());
        case 1: return bool2string(pp->getIsAccepted());
        default: return "";
    }
}

bool MTreeBoneNeighborRequestResponsePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestResponsePacket *pp = (MTreeBoneNeighborRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripeNumber(string2ulong(value)); return true;
        case 1: pp->setIsAccepted(string2bool(value)); return true;
        default: return false;
    }
}

const char *MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBoneNeighborRequestResponsePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneNeighborRequestResponsePacket *pp = (MTreeBoneNeighborRequestResponsePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBoneParentRequestPacket);

MTreeBoneParentRequestPacket::MTreeBoneParentRequestPacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_PARENT_REQUEST);

    this->stripeNumber_var = 0;
    this->abort_var = 0;
    this->ownHead_var = 0;
}

MTreeBoneParentRequestPacket::MTreeBoneParentRequestPacket(const MTreeBoneParentRequestPacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBoneParentRequestPacket::~MTreeBoneParentRequestPacket()
{
}

MTreeBoneParentRequestPacket& MTreeBoneParentRequestPacket::operator=(const MTreeBoneParentRequestPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBoneParentRequestPacket::copy(const MTreeBoneParentRequestPacket& other)
{
    this->stripeNumber_var = other.stripeNumber_var;
    this->abort_var = other.abort_var;
    this->ownHead_var = other.ownHead_var;
}

void MTreeBoneParentRequestPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    doPacking(b,this->stripeNumber_var);
    doPacking(b,this->abort_var);
    doPacking(b,this->ownHead_var);
}

void MTreeBoneParentRequestPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    doUnpacking(b,this->stripeNumber_var);
    doUnpacking(b,this->abort_var);
    doUnpacking(b,this->ownHead_var);
}

unsigned int MTreeBoneParentRequestPacket::getStripeNumber() const
{
    return stripeNumber_var;
}

void MTreeBoneParentRequestPacket::setStripeNumber(unsigned int stripeNumber)
{
    this->stripeNumber_var = stripeNumber;
}

bool MTreeBoneParentRequestPacket::getAbort() const
{
    return abort_var;
}

void MTreeBoneParentRequestPacket::setAbort(bool abort)
{
    this->abort_var = abort;
}

unsigned int MTreeBoneParentRequestPacket::getOwnHead() const
{
    return ownHead_var;
}

void MTreeBoneParentRequestPacket::setOwnHead(unsigned int ownHead)
{
    this->ownHead_var = ownHead;
}

class MTreeBoneParentRequestPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBoneParentRequestPacketDescriptor();
    virtual ~MTreeBoneParentRequestPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBoneParentRequestPacketDescriptor);

MTreeBoneParentRequestPacketDescriptor::MTreeBoneParentRequestPacketDescriptor() : cClassDescriptor("MTreeBoneParentRequestPacket", "MTreeBonePacketTree")
{
}

MTreeBoneParentRequestPacketDescriptor::~MTreeBoneParentRequestPacketDescriptor()
{
}

bool MTreeBoneParentRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBoneParentRequestPacket *>(obj)!=NULL;
}

const char *MTreeBoneParentRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBoneParentRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int MTreeBoneParentRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBoneParentRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripeNumber",
        "abort",
        "ownHead",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int MTreeBoneParentRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripeNumber")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "abort")==0) return base+1;
    if (fieldName[0]=='o' && strcmp(fieldName, "ownHead")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBoneParentRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "bool",
        "unsigned int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBoneParentRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBoneParentRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestPacket *pp = (MTreeBoneParentRequestPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBoneParentRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestPacket *pp = (MTreeBoneParentRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getStripeNumber());
        case 1: return bool2string(pp->getAbort());
        case 2: return ulong2string(pp->getOwnHead());
        default: return "";
    }
}

bool MTreeBoneParentRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestPacket *pp = (MTreeBoneParentRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripeNumber(string2ulong(value)); return true;
        case 1: pp->setAbort(string2bool(value)); return true;
        case 2: pp->setOwnHead(string2ulong(value)); return true;
        default: return false;
    }
}

const char *MTreeBoneParentRequestPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *MTreeBoneParentRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestPacket *pp = (MTreeBoneParentRequestPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBoneParentRequestResponsePacket);

MTreeBoneParentRequestResponsePacket::MTreeBoneParentRequestResponsePacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_PARENT_REQUEST_RESPONSE);

    this->stripeNumber_var = 0;
    this->isAccepted_var = 0;
}

MTreeBoneParentRequestResponsePacket::MTreeBoneParentRequestResponsePacket(const MTreeBoneParentRequestResponsePacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBoneParentRequestResponsePacket::~MTreeBoneParentRequestResponsePacket()
{
}

MTreeBoneParentRequestResponsePacket& MTreeBoneParentRequestResponsePacket::operator=(const MTreeBoneParentRequestResponsePacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBoneParentRequestResponsePacket::copy(const MTreeBoneParentRequestResponsePacket& other)
{
    this->stripeNumber_var = other.stripeNumber_var;
    this->isAccepted_var = other.isAccepted_var;
}

void MTreeBoneParentRequestResponsePacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    doPacking(b,this->stripeNumber_var);
    doPacking(b,this->isAccepted_var);
}

void MTreeBoneParentRequestResponsePacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    doUnpacking(b,this->stripeNumber_var);
    doUnpacking(b,this->isAccepted_var);
}

unsigned int MTreeBoneParentRequestResponsePacket::getStripeNumber() const
{
    return stripeNumber_var;
}

void MTreeBoneParentRequestResponsePacket::setStripeNumber(unsigned int stripeNumber)
{
    this->stripeNumber_var = stripeNumber;
}

bool MTreeBoneParentRequestResponsePacket::getIsAccepted() const
{
    return isAccepted_var;
}

void MTreeBoneParentRequestResponsePacket::setIsAccepted(bool isAccepted)
{
    this->isAccepted_var = isAccepted;
}

class MTreeBoneParentRequestResponsePacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBoneParentRequestResponsePacketDescriptor();
    virtual ~MTreeBoneParentRequestResponsePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBoneParentRequestResponsePacketDescriptor);

MTreeBoneParentRequestResponsePacketDescriptor::MTreeBoneParentRequestResponsePacketDescriptor() : cClassDescriptor("MTreeBoneParentRequestResponsePacket", "MTreeBonePacketTree")
{
}

MTreeBoneParentRequestResponsePacketDescriptor::~MTreeBoneParentRequestResponsePacketDescriptor()
{
}

bool MTreeBoneParentRequestResponsePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBoneParentRequestResponsePacket *>(obj)!=NULL;
}

const char *MTreeBoneParentRequestResponsePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBoneParentRequestResponsePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBoneParentRequestResponsePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBoneParentRequestResponsePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripeNumber",
        "isAccepted",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBoneParentRequestResponsePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripeNumber")==0) return base+0;
    if (fieldName[0]=='i' && strcmp(fieldName, "isAccepted")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBoneParentRequestResponsePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "bool",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBoneParentRequestResponsePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBoneParentRequestResponsePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestResponsePacket *pp = (MTreeBoneParentRequestResponsePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBoneParentRequestResponsePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestResponsePacket *pp = (MTreeBoneParentRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getStripeNumber());
        case 1: return bool2string(pp->getIsAccepted());
        default: return "";
    }
}

bool MTreeBoneParentRequestResponsePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestResponsePacket *pp = (MTreeBoneParentRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripeNumber(string2ulong(value)); return true;
        case 1: pp->setIsAccepted(string2bool(value)); return true;
        default: return false;
    }
}

const char *MTreeBoneParentRequestResponsePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBoneParentRequestResponsePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBoneParentRequestResponsePacket *pp = (MTreeBoneParentRequestResponsePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBonePeerInformationPacket);

MTreeBonePeerInformationPacket::MTreeBonePeerInformationPacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_PEER_INFORMATION);

    distance_arraysize = 0;
    this->distance_var = 0;
    numChildren_arraysize = 0;
    this->numChildren_var = 0;
}

MTreeBonePeerInformationPacket::MTreeBonePeerInformationPacket(const MTreeBonePeerInformationPacket& other) : MTreeBonePacketTree(other)
{
    distance_arraysize = 0;
    this->distance_var = 0;
    numChildren_arraysize = 0;
    this->numChildren_var = 0;
    copy(other);
}

MTreeBonePeerInformationPacket::~MTreeBonePeerInformationPacket()
{
    delete [] distance_var;
    delete [] numChildren_var;
}

MTreeBonePeerInformationPacket& MTreeBonePeerInformationPacket::operator=(const MTreeBonePeerInformationPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePeerInformationPacket::copy(const MTreeBonePeerInformationPacket& other)
{
    delete [] this->distance_var;
    this->distance_var = (other.distance_arraysize==0) ? NULL : new int[other.distance_arraysize];
    distance_arraysize = other.distance_arraysize;
    for (unsigned int i=0; i<distance_arraysize; i++)
        this->distance_var[i] = other.distance_var[i];
    delete [] this->numChildren_var;
    this->numChildren_var = (other.numChildren_arraysize==0) ? NULL : new int[other.numChildren_arraysize];
    numChildren_arraysize = other.numChildren_arraysize;
    for (unsigned int i=0; i<numChildren_arraysize; i++)
        this->numChildren_var[i] = other.numChildren_var[i];
}

void MTreeBonePeerInformationPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    b->pack(distance_arraysize);
    doPacking(b,this->distance_var,distance_arraysize);
    b->pack(numChildren_arraysize);
    doPacking(b,this->numChildren_var,numChildren_arraysize);
}

void MTreeBonePeerInformationPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    delete [] this->distance_var;
    b->unpack(distance_arraysize);
    if (distance_arraysize==0) {
        this->distance_var = 0;
    } else {
        this->distance_var = new int[distance_arraysize];
        doUnpacking(b,this->distance_var,distance_arraysize);
    }
    delete [] this->numChildren_var;
    b->unpack(numChildren_arraysize);
    if (numChildren_arraysize==0) {
        this->numChildren_var = 0;
    } else {
        this->numChildren_var = new int[numChildren_arraysize];
        doUnpacking(b,this->numChildren_var,numChildren_arraysize);
    }
}

void MTreeBonePeerInformationPacket::setDistanceArraySize(unsigned int size)
{
    int *distance_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = distance_arraysize < size ? distance_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        distance_var2[i] = this->distance_var[i];
    for (unsigned int i=sz; i<size; i++)
        distance_var2[i] = 0;
    distance_arraysize = size;
    delete [] this->distance_var;
    this->distance_var = distance_var2;
}

unsigned int MTreeBonePeerInformationPacket::getDistanceArraySize() const
{
    return distance_arraysize;
}

int MTreeBonePeerInformationPacket::getDistance(unsigned int k) const
{
    if (k>=distance_arraysize) throw cRuntimeError("Array of size %d indexed by %d", distance_arraysize, k);
    return distance_var[k];
}

void MTreeBonePeerInformationPacket::setDistance(unsigned int k, int distance)
{
    if (k>=distance_arraysize) throw cRuntimeError("Array of size %d indexed by %d", distance_arraysize, k);
    this->distance_var[k] = distance;
}

void MTreeBonePeerInformationPacket::setNumChildrenArraySize(unsigned int size)
{
    int *numChildren_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = numChildren_arraysize < size ? numChildren_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        numChildren_var2[i] = this->numChildren_var[i];
    for (unsigned int i=sz; i<size; i++)
        numChildren_var2[i] = 0;
    numChildren_arraysize = size;
    delete [] this->numChildren_var;
    this->numChildren_var = numChildren_var2;
}

unsigned int MTreeBonePeerInformationPacket::getNumChildrenArraySize() const
{
    return numChildren_arraysize;
}

int MTreeBonePeerInformationPacket::getNumChildren(unsigned int k) const
{
    if (k>=numChildren_arraysize) throw cRuntimeError("Array of size %d indexed by %d", numChildren_arraysize, k);
    return numChildren_var[k];
}

void MTreeBonePeerInformationPacket::setNumChildren(unsigned int k, int numChildren)
{
    if (k>=numChildren_arraysize) throw cRuntimeError("Array of size %d indexed by %d", numChildren_arraysize, k);
    this->numChildren_var[k] = numChildren;
}

class MTreeBonePeerInformationPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePeerInformationPacketDescriptor();
    virtual ~MTreeBonePeerInformationPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePeerInformationPacketDescriptor);

MTreeBonePeerInformationPacketDescriptor::MTreeBonePeerInformationPacketDescriptor() : cClassDescriptor("MTreeBonePeerInformationPacket", "MTreeBonePacketTree")
{
}

MTreeBonePeerInformationPacketDescriptor::~MTreeBonePeerInformationPacketDescriptor()
{
}

bool MTreeBonePeerInformationPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePeerInformationPacket *>(obj)!=NULL;
}

const char *MTreeBonePeerInformationPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePeerInformationPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBonePeerInformationPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBonePeerInformationPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "distance",
        "numChildren",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBonePeerInformationPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "distance")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "numChildren")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePeerInformationPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBonePeerInformationPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePeerInformationPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformationPacket *pp = (MTreeBonePeerInformationPacket *)object; (void)pp;
    switch (field) {
        case 0: return pp->getDistanceArraySize();
        case 1: return pp->getNumChildrenArraySize();
        default: return 0;
    }
}

std::string MTreeBonePeerInformationPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformationPacket *pp = (MTreeBonePeerInformationPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getDistance(i));
        case 1: return long2string(pp->getNumChildren(i));
        default: return "";
    }
}

bool MTreeBonePeerInformationPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformationPacket *pp = (MTreeBonePeerInformationPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setDistance(i,string2long(value)); return true;
        case 1: pp->setNumChildren(i,string2long(value)); return true;
        default: return false;
    }
}

const char *MTreeBonePeerInformationPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBonePeerInformationPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformationPacket *pp = (MTreeBonePeerInformationPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MTreeBonePeerSwitchPostionRequestPacket);

MTreeBonePeerSwitchPostionRequestPacket::MTreeBonePeerSwitchPostionRequestPacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_SWITCH_POSITION_REQUEST);

    this->stripe_var = 0;
}

MTreeBonePeerSwitchPostionRequestPacket::MTreeBonePeerSwitchPostionRequestPacket(const MTreeBonePeerSwitchPostionRequestPacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBonePeerSwitchPostionRequestPacket::~MTreeBonePeerSwitchPostionRequestPacket()
{
}

MTreeBonePeerSwitchPostionRequestPacket& MTreeBonePeerSwitchPostionRequestPacket::operator=(const MTreeBonePeerSwitchPostionRequestPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePeerSwitchPostionRequestPacket::copy(const MTreeBonePeerSwitchPostionRequestPacket& other)
{
    this->stripe_var = other.stripe_var;
    this->suggestedNewParent_var = other.suggestedNewParent_var;
}

void MTreeBonePeerSwitchPostionRequestPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    doPacking(b,this->stripe_var);
    doPacking(b,this->suggestedNewParent_var);
}

void MTreeBonePeerSwitchPostionRequestPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    doUnpacking(b,this->stripe_var);
    doUnpacking(b,this->suggestedNewParent_var);
}

int MTreeBonePeerSwitchPostionRequestPacket::getStripe() const
{
    return stripe_var;
}

void MTreeBonePeerSwitchPostionRequestPacket::setStripe(int stripe)
{
    this->stripe_var = stripe;
}

IPvXAddress& MTreeBonePeerSwitchPostionRequestPacket::getSuggestedNewParent()
{
    return suggestedNewParent_var;
}

void MTreeBonePeerSwitchPostionRequestPacket::setSuggestedNewParent(const IPvXAddress& suggestedNewParent)
{
    this->suggestedNewParent_var = suggestedNewParent;
}

class MTreeBonePeerSwitchPostionRequestPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePeerSwitchPostionRequestPacketDescriptor();
    virtual ~MTreeBonePeerSwitchPostionRequestPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePeerSwitchPostionRequestPacketDescriptor);

MTreeBonePeerSwitchPostionRequestPacketDescriptor::MTreeBonePeerSwitchPostionRequestPacketDescriptor() : cClassDescriptor("MTreeBonePeerSwitchPostionRequestPacket", "MTreeBonePacketTree")
{
}

MTreeBonePeerSwitchPostionRequestPacketDescriptor::~MTreeBonePeerSwitchPostionRequestPacketDescriptor()
{
}

bool MTreeBonePeerSwitchPostionRequestPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePeerSwitchPostionRequestPacket *>(obj)!=NULL;
}

const char *MTreeBonePeerSwitchPostionRequestPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripe",
        "suggestedNewParent",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBonePeerSwitchPostionRequestPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripe")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "suggestedNewParent")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePeerSwitchPostionRequestPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestPacket *pp = (MTreeBonePeerSwitchPostionRequestPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestPacket *pp = (MTreeBonePeerSwitchPostionRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getStripe());
        case 1: {std::stringstream out; out << pp->getSuggestedNewParent(); return out.str();}
        default: return "";
    }
}

bool MTreeBonePeerSwitchPostionRequestPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestPacket *pp = (MTreeBonePeerSwitchPostionRequestPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripe(string2long(value)); return true;
        default: return false;
    }
}

const char *MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBonePeerSwitchPostionRequestPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestPacket *pp = (MTreeBonePeerSwitchPostionRequestPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getSuggestedNewParent()); break;
        default: return NULL;
    }
}

Register_Class(MTreeBonePeerSwitchPostionRequestResponsePacket);

MTreeBonePeerSwitchPostionRequestResponsePacket::MTreeBonePeerSwitchPostionRequestResponsePacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE);

    this->stripe_var = 0;
}

MTreeBonePeerSwitchPostionRequestResponsePacket::MTreeBonePeerSwitchPostionRequestResponsePacket(const MTreeBonePeerSwitchPostionRequestResponsePacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBonePeerSwitchPostionRequestResponsePacket::~MTreeBonePeerSwitchPostionRequestResponsePacket()
{
}

MTreeBonePeerSwitchPostionRequestResponsePacket& MTreeBonePeerSwitchPostionRequestResponsePacket::operator=(const MTreeBonePeerSwitchPostionRequestResponsePacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePeerSwitchPostionRequestResponsePacket::copy(const MTreeBonePeerSwitchPostionRequestResponsePacket& other)
{
    this->stripe_var = other.stripe_var;
    this->newParent_var = other.newParent_var;
}

void MTreeBonePeerSwitchPostionRequestResponsePacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    doPacking(b,this->stripe_var);
    doPacking(b,this->newParent_var);
}

void MTreeBonePeerSwitchPostionRequestResponsePacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    doUnpacking(b,this->stripe_var);
    doUnpacking(b,this->newParent_var);
}

int MTreeBonePeerSwitchPostionRequestResponsePacket::getStripe() const
{
    return stripe_var;
}

void MTreeBonePeerSwitchPostionRequestResponsePacket::setStripe(int stripe)
{
    this->stripe_var = stripe;
}

IPvXAddress& MTreeBonePeerSwitchPostionRequestResponsePacket::getNewParent()
{
    return newParent_var;
}

void MTreeBonePeerSwitchPostionRequestResponsePacket::setNewParent(const IPvXAddress& newParent)
{
    this->newParent_var = newParent;
}

class MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor();
    virtual ~MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor);

MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor() : cClassDescriptor("MTreeBonePeerSwitchPostionRequestResponsePacket", "MTreeBonePacketTree")
{
}

MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::~MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor()
{
}

bool MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePeerSwitchPostionRequestResponsePacket *>(obj)!=NULL;
}

const char *MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripe",
        "newParent",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripe")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "newParent")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestResponsePacket *pp = (MTreeBonePeerSwitchPostionRequestResponsePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestResponsePacket *pp = (MTreeBonePeerSwitchPostionRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getStripe());
        case 1: {std::stringstream out; out << pp->getNewParent(); return out.str();}
        default: return "";
    }
}

bool MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestResponsePacket *pp = (MTreeBonePeerSwitchPostionRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripe(string2long(value)); return true;
        default: return false;
    }
}

const char *MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBonePeerSwitchPostionRequestResponsePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerSwitchPostionRequestResponsePacket *pp = (MTreeBonePeerSwitchPostionRequestResponsePacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getNewParent()); break;
        default: return NULL;
    }
}

Register_Class(MTreeBonePeerReplaceChildPacket);

MTreeBonePeerReplaceChildPacket::MTreeBonePeerReplaceChildPacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_REPLACE_CHILD);

    this->stripe_var = 0;
}

MTreeBonePeerReplaceChildPacket::MTreeBonePeerReplaceChildPacket(const MTreeBonePeerReplaceChildPacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBonePeerReplaceChildPacket::~MTreeBonePeerReplaceChildPacket()
{
}

MTreeBonePeerReplaceChildPacket& MTreeBonePeerReplaceChildPacket::operator=(const MTreeBonePeerReplaceChildPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePeerReplaceChildPacket::copy(const MTreeBonePeerReplaceChildPacket& other)
{
    this->stripe_var = other.stripe_var;
    this->newChild_var = other.newChild_var;
}

void MTreeBonePeerReplaceChildPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    doPacking(b,this->stripe_var);
    doPacking(b,this->newChild_var);
}

void MTreeBonePeerReplaceChildPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    doUnpacking(b,this->stripe_var);
    doUnpacking(b,this->newChild_var);
}

int MTreeBonePeerReplaceChildPacket::getStripe() const
{
    return stripe_var;
}

void MTreeBonePeerReplaceChildPacket::setStripe(int stripe)
{
    this->stripe_var = stripe;
}

IPvXAddress& MTreeBonePeerReplaceChildPacket::getNewChild()
{
    return newChild_var;
}

void MTreeBonePeerReplaceChildPacket::setNewChild(const IPvXAddress& newChild)
{
    this->newChild_var = newChild;
}

class MTreeBonePeerReplaceChildPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePeerReplaceChildPacketDescriptor();
    virtual ~MTreeBonePeerReplaceChildPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePeerReplaceChildPacketDescriptor);

MTreeBonePeerReplaceChildPacketDescriptor::MTreeBonePeerReplaceChildPacketDescriptor() : cClassDescriptor("MTreeBonePeerReplaceChildPacket", "MTreeBonePacketTree")
{
}

MTreeBonePeerReplaceChildPacketDescriptor::~MTreeBonePeerReplaceChildPacketDescriptor()
{
}

bool MTreeBonePeerReplaceChildPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePeerReplaceChildPacket *>(obj)!=NULL;
}

const char *MTreeBonePeerReplaceChildPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePeerReplaceChildPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBonePeerReplaceChildPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBonePeerReplaceChildPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripe",
        "newChild",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBonePeerReplaceChildPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripe")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "newChild")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePeerReplaceChildPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBonePeerReplaceChildPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePeerReplaceChildPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerReplaceChildPacket *pp = (MTreeBonePeerReplaceChildPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePeerReplaceChildPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerReplaceChildPacket *pp = (MTreeBonePeerReplaceChildPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getStripe());
        case 1: {std::stringstream out; out << pp->getNewChild(); return out.str();}
        default: return "";
    }
}

bool MTreeBonePeerReplaceChildPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerReplaceChildPacket *pp = (MTreeBonePeerReplaceChildPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripe(string2long(value)); return true;
        default: return false;
    }
}

const char *MTreeBonePeerReplaceChildPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBonePeerReplaceChildPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerReplaceChildPacket *pp = (MTreeBonePeerReplaceChildPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getNewChild()); break;
        default: return NULL;
    }
}

Register_Class(MTreeBonePeerInformNewParentPacket);

MTreeBonePeerInformNewParentPacket::MTreeBonePeerInformNewParentPacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_INFORM_NEW_PARENT);

    this->stripe_var = 0;
}

MTreeBonePeerInformNewParentPacket::MTreeBonePeerInformNewParentPacket(const MTreeBonePeerInformNewParentPacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBonePeerInformNewParentPacket::~MTreeBonePeerInformNewParentPacket()
{
}

MTreeBonePeerInformNewParentPacket& MTreeBonePeerInformNewParentPacket::operator=(const MTreeBonePeerInformNewParentPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePeerInformNewParentPacket::copy(const MTreeBonePeerInformNewParentPacket& other)
{
    this->stripe_var = other.stripe_var;
    this->newParent_var = other.newParent_var;
}

void MTreeBonePeerInformNewParentPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
    doPacking(b,this->stripe_var);
    doPacking(b,this->newParent_var);
}

void MTreeBonePeerInformNewParentPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
    doUnpacking(b,this->stripe_var);
    doUnpacking(b,this->newParent_var);
}

int MTreeBonePeerInformNewParentPacket::getStripe() const
{
    return stripe_var;
}

void MTreeBonePeerInformNewParentPacket::setStripe(int stripe)
{
    this->stripe_var = stripe;
}

IPvXAddress& MTreeBonePeerInformNewParentPacket::getNewParent()
{
    return newParent_var;
}

void MTreeBonePeerInformNewParentPacket::setNewParent(const IPvXAddress& newParent)
{
    this->newParent_var = newParent;
}

class MTreeBonePeerInformNewParentPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePeerInformNewParentPacketDescriptor();
    virtual ~MTreeBonePeerInformNewParentPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePeerInformNewParentPacketDescriptor);

MTreeBonePeerInformNewParentPacketDescriptor::MTreeBonePeerInformNewParentPacketDescriptor() : cClassDescriptor("MTreeBonePeerInformNewParentPacket", "MTreeBonePacketTree")
{
}

MTreeBonePeerInformNewParentPacketDescriptor::~MTreeBonePeerInformNewParentPacketDescriptor()
{
}

bool MTreeBonePeerInformNewParentPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePeerInformNewParentPacket *>(obj)!=NULL;
}

const char *MTreeBonePeerInformNewParentPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePeerInformNewParentPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MTreeBonePeerInformNewParentPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MTreeBonePeerInformNewParentPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "stripe",
        "newParent",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MTreeBonePeerInformNewParentPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "stripe")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "newParent")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePeerInformNewParentPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MTreeBonePeerInformNewParentPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePeerInformNewParentPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformNewParentPacket *pp = (MTreeBonePeerInformNewParentPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePeerInformNewParentPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformNewParentPacket *pp = (MTreeBonePeerInformNewParentPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getStripe());
        case 1: {std::stringstream out; out << pp->getNewParent(); return out.str();}
        default: return "";
    }
}

bool MTreeBonePeerInformNewParentPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformNewParentPacket *pp = (MTreeBonePeerInformNewParentPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setStripe(string2long(value)); return true;
        default: return false;
    }
}

const char *MTreeBonePeerInformNewParentPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "IPvXAddress",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MTreeBonePeerInformNewParentPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePeerInformNewParentPacket *pp = (MTreeBonePeerInformNewParentPacket *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getNewParent()); break;
        default: return NULL;
    }
}

Register_Class(MTreeBonePromotionPacket);

MTreeBonePromotionPacket::MTreeBonePromotionPacket(const char *name, int kind) : MTreeBonePacketTree(name,kind)
{
    this->setPacketType(MTREEBONE_PROMOTION);
}

MTreeBonePromotionPacket::MTreeBonePromotionPacket(const MTreeBonePromotionPacket& other) : MTreeBonePacketTree(other)
{
    copy(other);
}

MTreeBonePromotionPacket::~MTreeBonePromotionPacket()
{
}

MTreeBonePromotionPacket& MTreeBonePromotionPacket::operator=(const MTreeBonePromotionPacket& other)
{
    if (this==&other) return *this;
    MTreeBonePacketTree::operator=(other);
    copy(other);
    return *this;
}

void MTreeBonePromotionPacket::copy(const MTreeBonePromotionPacket& other)
{
}

void MTreeBonePromotionPacket::parsimPack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimPack(b);
}

void MTreeBonePromotionPacket::parsimUnpack(cCommBuffer *b)
{
    MTreeBonePacketTree::parsimUnpack(b);
}

class MTreeBonePromotionPacketDescriptor : public cClassDescriptor
{
  public:
    MTreeBonePromotionPacketDescriptor();
    virtual ~MTreeBonePromotionPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MTreeBonePromotionPacketDescriptor);

MTreeBonePromotionPacketDescriptor::MTreeBonePromotionPacketDescriptor() : cClassDescriptor("MTreeBonePromotionPacket", "MTreeBonePacketTree")
{
}

MTreeBonePromotionPacketDescriptor::~MTreeBonePromotionPacketDescriptor()
{
}

bool MTreeBonePromotionPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MTreeBonePromotionPacket *>(obj)!=NULL;
}

const char *MTreeBonePromotionPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MTreeBonePromotionPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int MTreeBonePromotionPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *MTreeBonePromotionPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int MTreeBonePromotionPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MTreeBonePromotionPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *MTreeBonePromotionPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MTreeBonePromotionPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePromotionPacket *pp = (MTreeBonePromotionPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MTreeBonePromotionPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePromotionPacket *pp = (MTreeBonePromotionPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool MTreeBonePromotionPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePromotionPacket *pp = (MTreeBonePromotionPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *MTreeBonePromotionPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *MTreeBonePromotionPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MTreeBonePromotionPacket *pp = (MTreeBonePromotionPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


