package so.hybrid.mtreebone;

import so.hybrid.mtreebone.MTreeBoneBase;

simple MTreeBonePeer extends MTreeBoneBase
{
    parameters:
        @class(MTreeBonePeer);
        
        bool disablePush = default(false);
        int ScheduleInterval = default(15);
}


