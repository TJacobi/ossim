package so.simulations;

import ned.DatarateChannel;

import so.global.ActivePeerTable;
import so.global.churn.IChurnModel;
import so.global.churn.StaticChurn;
import so.global.statistic.IStreamingStatistic;

import so.global.observer.MeshOverlayObserver;
import so.global.logger.Logger;

import so.global.AppSettingDonet;

import so.mesh.donet.DonetPeerNode;
import so.mesh.donet.DonetSourceNode;

import inet.nodes.inet.Router;
import inet.nodes.ethernet.EtherSwitch;
import inet.networklayer.autorouting.FlatNetworkConfigurator;


import so.hybrid.mtreebone.MTreeBoneSourceNode;
import so.hybrid.mtreebone.MTreeBonePeerNode;
import so.hybrid.mtreebone.MTreeBoneStats;
import so.hybrid.mtreebone.MTreeBoneSettings;
import so.hybrid.mtreebone.attacker.MTreeBoneSimpleAttackerNode;

import so.membership.tracker.GossipingTracker;

network MTreeBone_Homo_oneRouter_Network extends BasicNetwork
{
    parameters:
        int nNodePerRouter = default(1);
		int nSimpleAttackers = default(0);
		
		double minUploadPeer @unit("kbps") = default(1600kbps);
		double maxUploadPeer @unit("kbps") = default(4800kbps);
		
        @display("bgb=488,389");
        string churnModel = default("StaticChurn");
        string statisticModule = default("DonetStatistic");

    types:
        channel Channel_9M_up extends ned.DatarateChannel
        {
            datarate = 9Mbps;
        }
        channel Channel_9M_down extends ned.DatarateChannel
        {
            datarate = 40Mbps;
        }
        channel Channel_homo_up extends ned.DatarateChannel
        {
            //datarate = uniform(800kbps,1600kbps );//2-4
            datarate = uniform(minUploadPeer, maxUploadPeer );//4-12
        }
        channel Channel_homo_down extends ned.DatarateChannel
        {
            datarate = 2Mbps;
        }
    submodules:
        sourceNode: MTreeBoneSourceNode {
            @display("p=388,50;i=device/server");
        }

        peerNode[nNodePerRouter]: MTreeBonePeerNode;
		attackerNode[nSimpleAttackers]: MTreeBoneSimpleAttackerNode;
		
        router: Router {
            @display("p=329,136");
        }

        flatNetConfig: FlatNetworkConfigurator {
            @display("p=77,29;i=block/cogwheel");
            networkAddress = "192.168.0.0";
            netmask = "255.255.0.0";
        }

        activePeerTable: ActivePeerTable {
            @display("p=77,96");
        }

        churnModerator: <churnModel> like IChurnModel {
            @display("p=77,171");
        }
        
        MTreeBoneStats: MTreeBoneStats{}
		
		MTreeBoneSettings: MTreeBoneSettings{}
		
		GossipingTracker: GossipingTracker{}

        globalStatistic: <statisticModule> like IStreamingStatistic {
            @display("p=77,248;i=device/disk");
        }
        //        globalStatisic: Statistic {
        //            @display("p=179,328");
        //        }
        appSetting: AppSettingDonet {
            @display("p=77,328;i=block/wheelbarrow");
        }
        meshObserver: MeshOverlayObserver {
            @display("p=184,29");
        }
        logger: Logger {
        }
    connections:
        // -- old set-up
//        sourceNode.pppg$o++ --> Channel0_up --> router.pppg$i++;
//        sourceNode.pppg$i++ <-- Channel0_down <-- router.pppg$o++;
//        for i=0..nNodePerRouter-1 {
//            peerNode[i].pppg++ <--> ethernetline <--> router.pppg++;
//        }

        // -- new set-up
        sourceNode.pppg$o++ --> Channel_9M_up --> router.pppg$i++;
        sourceNode.pppg$i++ <-- Channel_9M_down <-- router.pppg$o++;
        for i=0..nNodePerRouter-1 {
            peerNode[i].pppg$o++ --> Channel_homo_up --> router.pppg$i++;
            peerNode[i].pppg$i++ <-- Channel_homo_down <-- router.pppg$o++;
        }
		for i=0..nSimpleAttackers-1 {
            attackerNode[i].pppg$o++ --> Channel_homo_up --> router.pppg$i++;
            attackerNode[i].pppg$i++ <-- Channel_homo_down <-- router.pppg$o++;
        }
}
