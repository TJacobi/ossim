package so.global.topology;

import so.global.topology.OverlayTopology;

simple OverlayTopologyDonet extends OverlayTopology
{
    parameters:
        @class(OverlayTopologyDonet);

        // -- Added by Giang
        //string appSetting = default("AppSettingDonet");
        string sourceAddress = default("192.168.0.1");
        //int observedChunk = default(1);
        int observeTime @unit(s) = default(1s);
}
