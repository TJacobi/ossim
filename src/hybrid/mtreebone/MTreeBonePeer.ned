package so.hybrid.mtreebone;

import so.hybrid.mtreebone.MTreeBoneBase;

simple MTreeBonePeer extends MTreeBoneBase
{
    parameters:
        @class(MTreeBonePeer);

        bool disablePush = default(false);
        //double ChunkScheduleInterval @unit("s") = default(0.75s);	// per peer needed?
        //double ChunkRequestTimeout @unit("s") = default(1.0s);		// per peer needed?
}


