#ifndef _PARAMS_INC
#define _PARAMS_INC
#pragma once

class Params
{
public:
	static const double PI;
	static int WindowWidth;
	static int WindowHeight;
	static int FramesPerSecond;

	static int NumSweeperInputs;
	static int NumSweeperHiddenLayers;
	static int SweeperNeuronsPerHiddenLayer;
	static int NumSweeperOutputs;

	static int NumKillerInputs;
	static int NumKillerHiddenLayers;
	static int KillerNeuronsPerHiddenLayer;
	static int NumKillerOutputs;

	static double ActivationResponse;
	static double Bias;
	
	static double MaxTurnRate;
	static double MaxSpeed;

	static double KillerScale;
	static int NumKillers;

	static double SweeperScale;
	static int NumSweepers;

	static double MineScale;
	static int NumMines;

	static int NumTicks;

	static double MutationRate;
	static double CrossoverRate;
	static double MaxPerturbation;
	static int NumElite;
	static int NumCopiesElite;

	Params(const char *_FileName = "params.ini");
	bool LoadInParameters(const char *_FileName);
};

#endif