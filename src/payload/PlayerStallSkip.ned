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
// Player.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

package so.payload;

simple PlayerStallSkip like PlayerBase
{
   parameters:
      @class(PlayerStallSkip);
      double interval_recheckVideoBuffer @unit(s) = default(0.1s);
      int max_skipped_chunk = default(5);
      int max_stalled_chunks = default(10);
      double interval_probe_playerStart @unit(s) = default(1s);
      
      double bufferSecondsBeforeStart = default(3.0);
      int minimumOfChunksReceivedBeforeStart = default(50);
        
		// --- Possible signals to collect
		@signal[rebuffering_Local](type=long);
		@signal[Signal_ChunkHit](type=long);
		@signal[Signal_ChunkMiss](type=long);
		@signal[Signal_ChunkSeek](type=long);
		
		// --- Statistics
      //@statistic[ChunkHit](source=Signal_ChunkHit; title="Number of chunk hit"; record=count,vector);
      //@statistic[ChunkMiss](source=Signal_ChunkMiss; title="Number of chunk Miss"; record=count,vector);
      //@statistic[ChunkSeek](source=Signal_ChunkSeek; title="Number of chunk Seek"; record=count,vector);
      //@statistic[CI](title="Continuity Index"; source="count(Signal_ChunkHit)/count(Signal_ChunkSeek)"; record=last);

      //@statistic[rebuffering](title="Number of Re-buffering events"; source=Signal_Rebuffering; record=sum,vector);
      //@statistic[stall](title="Number of stall events"; source=Signal_Stall; record=sum,vector);

      //@statistic[timePlayerStart](source=Signal_timePlayerStart; record=first,last);

}


