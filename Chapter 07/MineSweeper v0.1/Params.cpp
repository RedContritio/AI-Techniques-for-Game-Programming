#include "Params.h"
#include "LogSpawner.h"

#include <cstdio>
#include <cassert>

#pragma warning(push)
#pragma warning(disable: 4996)

extern RedContritio::LogSpawner logger;

Params::Params(const char *_FileName)
{
	if ( !LoadInParameters(_FileName) )
	{
		throw("Failed to load parameters.");
	}
	else
	{
		logger.printf("parameters loading end.\n");
	}
}

#define LoadParameter(name, type) do { \
	assert(0 < fscanf(fin, "%*s" type, &(this->name))); \
		logger.printf("para@" #name ": " type " \n", this->name); \
} while ( 0 );


bool Params::LoadInParameters(const char *_FileName)
{
	FILE *fin;
#ifdef __STDC_WANT_SECURE_LIB__
	fopen_s(&fin, _FileName, "r");
#else
	fin = fopen(_FileName, "r");
#endif
	if ( !fin ) return false;

	logger.printf("loading parameters\n");

	LoadParameter(FramesPerSecond, "%d");

	LoadParameter(NumInputs, "%d");
	LoadParameter(NumHiddenLayers, "%d");
	LoadParameter(NeuronsPerHiddenLayer, "%d");
	LoadParameter(NumOutputs, "%d");

	LoadParameter(ActivationResponse, "%lf");
	LoadParameter(Bias, "%lf");

	LoadParameter(MaxTurnRate, "%lf");
	LoadParameter(MaxSpeed, "%lf");

	LoadParameter(SweeperScale, "%lf");
	LoadParameter(NumSweepers, "%d");

	LoadParameter(MineScale, "%lf");
	LoadParameter(NumMines, "%d");

	LoadParameter(NumTicks, "%d");

	LoadParameter(MutationRate, "%lf");
	LoadParameter(CrossoverRate, "%lf");
	LoadParameter(MaxPerturbation, "%lf");

	LoadParameter(NumElite, "%d");
	LoadParameter(NumCopiesElite, "%d");
	
	fclose(fin);
	return true;
}


const double Params::PI = 3.14159265358979;


int Params::WindowWidth = 0;
int Params::WindowHeight = 0;

int Params::FramesPerSecond = 0;

int Params::NumInputs = 0;
int Params::NumHiddenLayers = 0;
int Params::NeuronsPerHiddenLayer = 0;
int Params::NumOutputs = 0;

double Params::ActivationResponse = 0;
double Params::Bias = 0;

double Params::MaxTurnRate = 0;
double Params::MaxSpeed = 0;

double Params::SweeperScale = 1;
int Params::NumSweepers = 0;

double Params::MineScale = 1;
int Params::NumMines = 0;

int Params::NumTicks = 0;

double Params::MutationRate = 0;
double Params::CrossoverRate = 0;
double Params::MaxPerturbation = 0;
int Params::NumElite = 0;
int Params::NumCopiesElite = 0;


#pragma warning(pop)