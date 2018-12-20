#ifndef __CPARAM__INC
#define __CPARAM__INC
#pragma once

struct CParam
{
	static int WindowWidth ;
	static int WindowHeight ;

	static int FramesPerSecond ;
	static int NumOfTicks ;

	static double Pi ;


	static int NumOfInput ;
	static int NumOfHidden ;
	static int NeuronsPerHiddenLayer ;
	static int NumOfOutput ;

	static double ActivationResponce ;
	static double BIAS ;


	static double MaxTurnRate ;
	static double MaxSpeed ;

	static double SweeperScale ;
	static double MineScale ;

	static double StartEnergy ;
	static double EnergyCostPerTick ;

	static int NumOfSweepers ;
	static int NumOfMines ;


	static double CrossoverRate ;
	static double MutationRate ;
	static double dMaxPerturbation;

	static int NumOfElite ;
	static int NumOfCopiesElite ;

	static void LoadParameters(void );
};

#endif