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
}}


class PeerStreamingPacket;

// Messages for the MTreeBone-Protocol
// @author Thorsten Jacobi

enum MTreeBonePacketType
{
	MTREEBONE_BUFFER_MAP = 0;
	MTREEBONE_CHUNK_REQUEST = 1;
	MTREEBONE_CHUNK_DENY = 2;
	MTREEBONE_NEIGHBOR_REQUEST = 3;
	MTREEBONE_NEIGHBOR_REQUEST_RESPONSE = 4;
}

packet MTreeBonePacket extends PeerStreamingPacket
{
    packetGroup = PACKET_GROUP_MESH_OVERLAY;
    
    short packetType     @getter(getPacketType) @setter(setPacketType) @enum(MTreeBonePacketType) ;     
}

packet MTreeBoneBufferMapPacket extends MTreeBonePacket
{
	packetType = MTREEBONE_BUFFER_MAP;
	
	unsigned int  sequenceNumberStart;
	unsigned int  sequenceNumberEnd;
	bool boneNodeForStripe[];
	bool bufferMap[];
}

packet MTreeBoneChunkRequestPacket extends MTreeBonePacket
{
	packetType = MTREEBONE_CHUNK_REQUEST;
	
	unsigned int sequenceNumber;
}

packet MTreeBoneChunkDenyPacket extends MTreeBonePacket
{
	packetType = MTREEBONE_CHUNK_DENY;
	
	unsigned int sequenceNumber;
	unsigned int requestThis;
}

packet MTreeBoneNeighborRequestPacket extends MTreeBonePacket
{
	packetType = MTREEBONE_NEIGHBOR_REQUEST;
	
	unsigned int stripeNumber;
}

packet MTreeBoneNeighborRequestResponsePacket extends MTreeBonePacket
{
	packetType = MTREEBONE_NEIGHBOR_REQUEST_RESPONSE;
	
	unsigned int stripeNumber;
	bool isAccepted;
}