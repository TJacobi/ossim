package so.hybrid.mtreebone;

import so.common.Dispatcher;
import so.membership.MCache;
import so.hybrid.mtreebone.MTreeBoneSource;
import so.mesh.PartnerList;
import so.payload.ChunkGenerator;
import so.payload.Forwarder;
import so.payload.VideoBuffer;
import so.membership.newscast.NewscastPeer;

import so.membership.dummy.DummyMembership;

module MTreeBoneSourceApp
{
    parameters:
        @display("bgb=287,308");

    gates:
        input udpIn;
        output udpOut;

    submodules:
        //membership: ScampPeer {
        mesh: MTreeBoneSource {
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
        
		//dummy1: DummyMembership {
        //    @display("p=51,173;i=block/users");
        //}
		dummy2: DummyMembership {
            @display("p=51,173;i=block/users");
        }
        
        gossipProtocol : NewscastPeer;
        
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
        dispatcher.overlayIn++ <-- mesh.dpOut;
        dispatcher.overlayOut++ --> mesh.dpIn;

        // Connect Forwarder module to the Dispatcher
        dispatcher.overlayIn++ <-- forwarder.dpOut;
        dispatcher.overlayOut++ --> forwarder.dpIn;

        // Connect tree module to the Dispatcher
        dispatcher.overlayIn++ <-- dummy2.dpOut;
        dispatcher.overlayOut++ --> dummy2.dpIn;
        
        // Connect hybrid module to the Dispatcher
        //dispatcher.overlayIn++ <-- mesh.dpOut;
        //dispatcher.overlayOut++ --> mesh.dpIn;
        
        // Connect Dispatcher to module's ports
        dispatcher.udpIn <-- udpIn;
        dispatcher.udpOut --> udpOut;

}
