package so.hybrid.mtreebone;

import so.hybrid.mtreebone.MTreeBoneBase;

simple MTreeBonePeer extends MTreeBoneBase
{
    parameters:
        @class(MTreeBonePeer);

        bool disablePush = default(false);
        double ChunkScheduleInterval @unit("s") = default(0.75s);	// per peer needed?
        double joinTime @unit("s") = default(uniform(0.5s,5.0s));
        double stayDuration @unit("s") = default(-1.0s);
        double ChunkRequestTimeout @unit("s") = default(1.0s);		// per peer needed?
}


