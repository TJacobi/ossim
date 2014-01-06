//  
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with 
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// MTreeBonePacket.msg
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// -----------------------------------------------------------------------------
//


cplusplus {{
#include <PeerStreamingPacket_m.h>
#include "IPvXAddress.h"
}}

class PeerStreamingPacket;
class noncobject IPvXAddress;

// Messages for the MTreeBone-Protocol
// @author Thorsten Jacobi

enum MTreeBonePacketType
{
	MTREEBONE_BUFFER_MAP = 0;
	//MTREEBONE_CHUNK_REQUEST = 1;
	//MTREEBONE_CHUNK_DENY = 2;
	MTREEBONE_NEIGHBOR_REQUEST = 3;
	MTREEBONE_NEIGHBOR_REQUEST_RESPONSE = 4;
	MTREEBONE_PARENT_REQUEST = 5;
	MTREEBONE_PARENT_REQUEST_RESPONSE = 6;
	MTREEBONE_CHUNK_REQUEST_LIST = 7;
	MTREEBONE_PEER_INFORMATION = 8;
	MTREEBONE_SWITCH_POSITION_REQUEST = 9;
	MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE = 10;
	MTREEBONE_REPLACE_CHILD = 11;
	MTREEBONE_INFORM_NEW_PARENT = 12;
	MTREEBONE_PROMOTION = 13;
	MTREEBONE_PACKETS_MAX = 13;
}

packet MTreeBonePacket extends PeerStreamingPacket
{

	short packetType     @getter(getPacketType) @setter(setPacketType) @enum(MTreeBonePacketType) ;
	
}


packet MTreeBonePacketMesh extends MTreeBonePacket
{
    packetGroup = PACKET_GROUP_MESH_OVERLAY;  
}

packet MTreeBonePacketTree extends MTreeBonePacket
{
    packetGroup = PACKET_GROUP_TREE_OVERLAY; 
}

packet MTreeBoneBufferMapPacket extends MTreeBonePacketMesh
{
	packetType = MTREEBONE_BUFFER_MAP;
	
	unsigned int  sequenceNumberStart;
	unsigned int  sequenceNumberEnd;
	unsigned int  missingChunks;
	bool boneNodeForStripe[];
	bool bufferMap[];
	bool tempYouAreMyParent;
}

packet MTreeBoneChunkRequestListPacket extends MTreeBonePacketMesh
{
	packetType = MTREEBONE_CHUNK_REQUEST_LIST;
	
	unsigned int sequenceNumbers[];
}

packet MTreeBoneNeighborRequestPacket extends MTreeBonePacketMesh
{
	packetType = MTREEBONE_NEIGHBOR_REQUEST;
	
	unsigned int stripeNumber;
	double		joinTime;
}

packet MTreeBoneNeighborRequestResponsePacket extends MTreeBonePacketMesh
{
	packetType = MTREEBONE_NEIGHBOR_REQUEST_RESPONSE;
	
	unsigned int stripeNumber;
	bool isAccepted;
}

packet MTreeBoneParentRequestPacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_PARENT_REQUEST;
	
	unsigned int stripeNumber;
	bool abort;
	
	unsigned int ownHead;
}

packet MTreeBoneParentRequestResponsePacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_PARENT_REQUEST_RESPONSE;
	
	unsigned int stripeNumber;
	bool isAccepted;
}

packet MTreeBonePeerInformationPacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_PEER_INFORMATION;
	
	int distance[];
	int numChildren[];
}

packet MTreeBonePeerSwitchPostionRequestPacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_SWITCH_POSITION_REQUEST;
	
	int stripe;
	IPvXAddress suggestedNewParent;
}

packet MTreeBonePeerSwitchPostionRequestResponsePacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_SWITCH_POSITION_REQUEST_RESPONSE;
	
	int stripe;
	IPvXAddress newParent;
}

packet MTreeBonePeerReplaceChildPacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_REPLACE_CHILD;
	
	int stripe;
	IPvXAddress newChild;
}

packet MTreeBonePeerInformNewParentPacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_INFORM_NEW_PARENT;
	
	int stripe;
	IPvXAddress newParent;
}

packet MTreeBonePromotionPacket extends MTreeBonePacketTree
{
	packetType = MTREEBONE_PROMOTION;
}

