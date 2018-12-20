#ifndef __CPARAM__DEF
#define __CPARAM__DEF
#pragma once
#include "defines.h"
#include <cstdio>
#include <windows.h>

class CPARAMETERS
{
	public :
		static double PI ;

		static int WindowWidth ;
		static int WindowHeight ;

		static int FramesPerSecond ;

		static int NumInputs ;
		static int NumHidden ;
		static int NeuronsPerHiddenLayer ;
		static int NumOutput ;

		static double ActivationResponse ;
		static double BIAS ;


		static double MaxTurnRate ;
		static double MaxSpeed ;

		static int SweeperScale ;
		static int MineScale ;

		static int NumSweepers ;
		static int NumMines ;
		static int NumMaxTicks ;

		static double CrossoverRate ;
		static double MutationRate ;

		static double MaxPerturbation ;
		static int NumElites ;
		static int NumCopiesElite ;

		CPARAMETERS(void );
		bool LoadedParaments(const char* );
};

enum PARAMETERSTYPE ;

struct PARAMETERASSIST
{
private :
	static double Fitness ;
public :
	PARAMETERASSIST(void );
	~PARAMETERASSIST(void );
	static const int NumParameters ;
	static void **pParams ;
	static void InitilizeParamsPointer(void );
	static char* ExistElement(char* , char );
	static void* FindParamAddress(const char* );
	static PARAMETERSTYPE GetParameterType(const void* );
};
#endif // __CPARAM__DEF