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
// NewscastBase.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

package so.membership.newscast;

import so.membership.MembershipBase;

//simple NewscastBase
simple NewscastBase extends MembershipBase
{
    parameters:
        @class(NewscastBase);

		// NEWSCAST protocol parameter
        int cacheSize = default(10); 
        
        // intervall for cache exchanges
        double exchangeCacheIntervall @unit(s) = default(5.0s);
 
        bool autoJoinNetwork = default(false);
//    gates:
//        // -- Gates to the Dispatcher module
//        input dpIn @labels(DpControlInfo/up);
//        output dpOut @labels(DpControlInfo/down);
}
