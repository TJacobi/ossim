// This is the global attacker module.

package so.global.attacker;


simple UnderlayAttackerModule
{
    parameters:
        bool active;
        int attackLength;
        int startAttack;
		int victimNumber;
		double attackPauseRatio = default(0.5);
		double truncateDamageMax = default(0.25);
		double truncateDamageStep = default(0.01);
		bool performDamageTest = default(true); //should be true to gather greedy damage
		double simTimeLimit; // variable that gives the end of the simulation
}
