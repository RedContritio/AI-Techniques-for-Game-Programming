#ifndef CPARAM__INC
#define CPARAM__INC
#pragma once
#ifndef NULL
#define NULL    0
#endif
struct CPARAM
{
	static double Pi ;

	static int WindowHeight ;
	static int WindowWidth ;
	static int FramePerSecond ;

	static int NumTicks ;

	static int NumSweepers ;

	static double MutationRate ;
	static double CrossoverRate ;
	static double MaxPerturbation ;
	static int NumCopiesElite ;
	static int NumElite ;

	static int NumInputs ;
	static int NumHidden ;
	static int NumOutputs ;
	static int NeuronsPerHiddenLayer ;
	static double BIAS ;
	static double ActivationResponse ;

	static double SweeperScale ;

	static int NumSensors ;
	static double SensorRange ;
	static double CollisionDist ;

	static int LoadParameters(const char* filename =NULL );
};
#endif