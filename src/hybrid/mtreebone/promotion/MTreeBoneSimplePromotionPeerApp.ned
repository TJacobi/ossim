package so.hybrid.mtreebone.promotion;

import so.common.Dispatcher;
import so.membership.MCache;

import so.hybrid.mtreebone.MTreeBonePeer;

import so.hybrid.mtreebone.MTreeBoneMesh;
//import so.hybrid.mtreebone.MTreeBoneTree;


import so.mesh.PartnerList;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.payload.PlayerStallSkip;
import so.payload.PlayerBufferSkip;
//import simpleoverlay.global.AppSettingDonet;
import so.hybrid.mtreebone.promotion.MTreeBoneTreeSimplePromotion;

import so.membership.newscast.NewscastPeer;
import so.membership.tracker.SimpleGossipingTrackerAdapter;

import so.membership.dummy.DummyMembership;

module MTreeBoneSimplePromotionPeerApp
{
    parameters:
        @display("bgb=270,294");
//        @statistic[hitCount_global](source=chunkHit_Global; record=count);

    gates:
        input udpIn;
        output udpOut;

    submodules:
        peer: MTreeBonePeer {
            @display("p=132,173;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=211,173;i=block/rxtx");
        }
        mCache: MCache {
            @display("p=51,104;i=abstract/cache");
        }
        videoBuffer: VideoBuffer {
            @display("p=211,104;i=block/buffer2");
        }
        player: PlayerBufferSkip {//PlayerStallSkip {
            @display("p=211,44;i=device/terminal");
        }
        partnerList: PartnerList {

            @display("p=132,104");
        }

        //gossipProtocol : NewscastPeer;
        gossipProtocol : SimpleGossipingTrackerAdapter;
        
        meshmodule : MTreeBoneMesh;
        treemodule : MTreeBoneTreeSimplePromotion;
        
        dispatcher: Dispatcher {
            @display("p=132,247;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=34,178;i=block/cogwheel");
    //        }
    connections:
        // Connect Forwarder module to the NewscastPeer
        dispatcher.overlayIn++ <-- gossipProtocol.dpOut;
        dispatcher.overlayOut++ --> gossipProtocol.dpIn;
        
        // Connect Mesh module to the Dispatcher
        dispatcher.overlayIn++ <-- meshmodule.dpOut;
        dispatcher.overlayOut++ --> meshmodule.dpIn;

        // Connect Forwarder module to the Dispatcher
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        // Connect tree module to the Dispatcher
        dispatcher.overlayIn++ <-- treemodule.dpOut;
        dispatcher.overlayOut++ --> treemodule.dpIn;
        
        // Add our peer module to Dispatcher
        dispatcher.overlayIn++ <-- peer.dpOut;
        dispatcher.overlayOut++ --> peer.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;
}
