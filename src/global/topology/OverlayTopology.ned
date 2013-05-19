package so.global.topology;

simple OverlayTopology
{
    parameters:
        @class(OverlayTopology);

        bool socketExport;
        int sendPort;
        int streamStripes;
        int lossMinStripes;
}
