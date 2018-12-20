#include "CParam.h"
#include <cstdio>

double CPARAM::Pi =3.1415926535 ;


int CPARAM::WindowWidth =640 ;
int CPARAM::WindowHeight =480 ;

int CPARAM::FramePerSecond =10 ;

int CPARAM::NumTicks =2000 ;

int CPARAM::NumSweepers =40 ;

int CPARAM::NumCopiesElite = 1 ;
int CPARAM::NumElite =4 ;
double CPARAM::MaxPerturbation =0.2 ;
double CPARAM::MutationRate =0.1 ;
double CPARAM::CrossoverRate =0.7 ;


int CPARAM::NumInputs =5 ;
int CPARAM::NumHidden =1 ;
int CPARAM::NumOutputs =2 ;
int CPARAM::NeuronsPerHiddenLayer =6 ;

double CPARAM::BIAS =-1 ;
double CPARAM::ActivationResponse =1 ;


double CPARAM::SweeperScale =5 ;

int CPARAM::NumSensors =5 ;
double CPARAM::SensorRange =25 ;
double CPARAM::CollisionDist =0 ;

int CPARAM::LoadParameters(const char* filename )
{
	FILE* fp_ini =NULL ;
	fopen_s(&fp_ini ,filename ,"r" );
	if(fp_ini ==NULL )return 0 ;
	int NumRead =0 ;


	fclose(fp_ini );
	return NumRead ;
}