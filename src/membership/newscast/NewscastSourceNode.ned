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
// NewscastSourceNode.ned
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//


package so.membership.newscast;

import so.membership.newscast.NewscastPeerApp;

import inet.applications.pingapp.PingApp;
import inet.applications.sctpapp.SCTPApp;
import inet.applications.tcpapp.TCPApp;
import inet.applications.udpapp.UDPApp;
import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ext.ExtInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.transport.sctp.SCTP;
import inet.transport.tcp.ITCP;
import inet.transport.udp.UDP;
import inet.util.NAMTraceWriter;
import inet.util.TCPDump;

import inet.nodes.inet.NetworkLayer;

import so.membership.newscast.NewscastSourceApp;
//
// \IP host with SCTP, TCP, UDP layers and applications.
//
module NewscastSourceNode
{
    parameters:
        @node();
        @labels(node,ethernet-node);
        @display("i=device/pc2");
        int numTcpApps = default(0);
        int numUdpApps = default(0);
        int numSctpApps = default(0);
        string tcpAppType = default("n/a");
        string udpAppType = default("n/a");
        string sctpAppType = default("n/a");
        string tcpType = default("TCP");  // tcp implementation (e.g. TCP, TCP_old, TCP_NSC)
        bool IPForward = default(false);
        int namid = default(-1);
        string routingFile = default("");
        int numExtInterfaces = default(0);
    gates:
        inout pppg[] @labels(PPPFrame-conn);
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=57,54");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=57,134");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = IPForward;
                routerId = "";
                routingFile = routingFile;
                @display("p=57,214");
        }
        
        NewscastSourceApp: NewscastSourceApp {
            parameters:
                @display("p=163,67");
        }
        udp: UDP {
            parameters:
                @display("p=336,141");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=336,230;q=queue");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=186,390,row,90;q=txQueue");
        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=336,390,row,90;q=txQueue");
        }
        tcpdump: TCPDump {
            parameters:
                @display("p=336,302;i=abstract/cache_s");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                in2[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
                out2[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        }
    connections allowunconnected:

        NewscastSourceApp.udpOut --> udp.appIn++;
        NewscastSourceApp.udpIn <-- udp.appOut++;

        udp.ipOut --> networkLayer.udpIn;
        udp.ipIn <-- networkLayer.udpOut;

        // connections to network outside
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> ppp[i].phys;
            ppp[i].netwOut --> tcpdump.ifIn[i];
            tcpdump.out2[i] --> networkLayer.ifIn[i];
            ppp[i].netwIn <-- tcpdump.ifOut[i];
            tcpdump.in2[i] <-- networkLayer.ifOut[i];
        }

        for i=0..sizeof(ethg)-1 {
            ethg[i] <--> eth[i].phys;
            eth[i].netwOut --> tcpdump.ifIn[sizeof(pppg)+i];
            tcpdump.out2[sizeof(pppg)+i] --> networkLayer.ifIn[sizeof(pppg)+i];
            eth[i].netwIn <-- tcpdump.ifOut[sizeof(pppg)+i];
            tcpdump.in2[sizeof(pppg)+i] <-- networkLayer.ifOut[sizeof(pppg)+i];
        }

        for i=0..numExtInterfaces-1 {
            //            ext[i].netwOut --> tcpdump.ifIn[sizeof(pppg)+sizeof(ethg)+i];
            tcpdump.out2[sizeof(pppg)+sizeof(ethg)+i] --> networkLayer.ifIn[sizeof(pppg)+sizeof(ethg)+i];
            //            ext[i].netwIn <-- tcpdump.ifOut[sizeof(pppg)+sizeof(ethg)+i];
            tcpdump.in2[sizeof(pppg)+sizeof(ethg)+i] <-- networkLayer.ifOut[sizeof(pppg)+sizeof(ethg)+i];
        }
}
