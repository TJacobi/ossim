
package so.hybrid.mtreebone.promotion;

import so.common.CommBase;
import so.hybrid.mtreebone.MTreeBoneTree;

simple MTreeBoneTreeSimplePromotion extends MTreeBoneTree 
{
   parameters:
		@class(MTreeBoneTreeSimplePromotion);
		
		bool useJoinTime = default(true);
}
