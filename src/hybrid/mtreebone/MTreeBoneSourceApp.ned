package so.hybrid.mtreebone;

import so.common.Dispatcher;
import so.membership.MCache;
import so.hybrid.mtreebone.MTreeBoneSource;
import so.mesh.PartnerList;
import so.payload.ChunkGenerator;
import so.payload.Forwarder;
import so.payload.VideoBuffer;

import so.membership.newscast.NewscastPeer;
import so.membership.tracker.SimpleGossipingTrackerAdapter;

import so.membership.dummy.DummyMembership;

module MTreeBoneSourceApp
{
    parameters:
        @display("bgb=287,308");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        source: MTreeBoneSource {
            @display("p=133,179;i=block/network2");
        }
        forwarder: Forwarder {
            @display("p=212,179;i=block/rxtx");
        }
        mCache: MCache {
            @display("p=52,110;i=abstract/cache");
        }
        videoBuffer: VideoBuffer {
            @display("p=212,110;i=block/buffer2");
        }
        chunkGenerator: ChunkGenerator {
            @display("p=212,37;i=block/source");
        }
        partnerList: PartnerList {

            @display("p=133,110");
        }
        
      
        //gossipProtocol : NewscastPeer;
        gossipProtocol : SimpleGossipingTrackerAdapter;
        
        meshmodule : MTreeBoneMesh;
        treemodule : MTreeBoneTree;
        
        dispatcher: Dispatcher {
            @display("p=133,253;i=block/dispatch");
        }
    //        appSetting: AppSettingDonet {
    //            @display("p=41,191");
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
        
        // Connect source module to the Dispatcher
        dispatcher.overlayIn++ <-- source.dpOut;
        dispatcher.overlayOut++ --> source.dpIn;

        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}
