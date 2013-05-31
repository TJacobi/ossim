//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//package p2pstreaming.base.inet;
package so.underlay.inet;

//
// TODO auto-generated module
//
simple ICMPMobility
{
    parameters:
        @display("i=block/control_s");
    gates:
        input 	localIn;  	// delivered ICMP packets
        input 	pingIn;  	// ping requests from app
        
        output 	pingOut;  	// result of ping
        output 	sendOut;  	// towards network
        
        output 	errorOut;  	// errors
        
        output 	to_trcrt;	// to traceroute        	
        input 	from_trcrt;	// from traceroute
}
