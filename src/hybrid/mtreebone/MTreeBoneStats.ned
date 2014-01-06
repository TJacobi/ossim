
package so.hybrid.mtreebone;

simple MTreeBoneStats
{
   parameters:
		@class(MTreeBoneStats);
		
	bool enablePeriodicalStructureOutput = default(true);
	double PeriodicalStructureOutputIntervall @unit(s) = default(5.0s);

	double PeriodicalContinuityIndexIntervall	@unit(s) = default(30.0s);
	double PeriodicalAttackedChildrenIntervall 	@unit(s) = default(30.0s);
	//string newscastGetRandomIPsLOG = default("newscastGetRandomIPs.txt");
	//string newscastConnectionsLOG = default("newscastConnections.txt");
}
