
package so.hybrid.mtreebone;

simple MTreeBoneSettings
{
   parameters:
		@class(MTreeBoneSettings);
		
		int NumberOfStripes = default(1);
		
		double ChunkScheduleInterval @unit("s") = default(0.75s);
        double ChunkRequestTimeout   @unit("s") = default(1.0s);
        
        double CheckNeighborsIntervall @unit("s") = default(1.0s);
        
        double BufferMapIntervall    @unit("s") = default(0.6s);
        
        int NumberOfPartnersMaximum  = default (16);
        int NumberOfPartnersDesired  = default(8);
        
        int kValueMode = default(4);
	//bool enablePeriodicalStructureOutput = default(true);

	//string newscastGetRandomIPsLOG = default("newscastGetRandomIPs.txt");
	//string newscastConnectionsLOG = default("newscastConnections.txt");
}
