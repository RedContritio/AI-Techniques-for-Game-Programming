#include "CParams.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>

using std::max ;

#include "LogSpawner.h"
using RedContritio::LogSpawner ;
extern LogSpawner loger ;
extern CPARAMETERS GlobalParameters ;

static int LCS(const char* str1 ,const char* str2 );

const char* ParametersName[]={"WindowWidth" ,
							 "WindowHeight" ,
							 "FramesPerSecond" ,
							 "NumInputs" ,
							 "NumHidden" ,
							 "NeuronsPerHiddenLayer" ,
							 "NumOutput" ,
							 "ActivationResponse" ,
							 "BIAS" ,
							 "MaxTurnRate" ,
							 "MaxSpeed" ,
							 "SweeperScale" ,
							 "MineScale" ,
							 "NumSweepers" ,
							 "NumMines" ,
							 "NumMaxTicks" ,
							 "CrossoverRate" ,
							 "MutationRate" ,
							 "MaxPerturbation" ,
							 "NumElites" ,
							 "NumCopiesElite" };

enum PARAMETERSTYPE
{
	type_int ,type_double ,type_error
};


double CPARAMETERS::PI = 3.1415926535897932384626433832795 ;

int CPARAMETERS::WindowWidth = __DEFAULT__WINDOW__WIDTH ;
int CPARAMETERS::WindowHeight = __DEFAULT__WINDOW__HEIGHT ;

int CPARAMETERS::FramesPerSecond = __DEFAULT__FRAME__PER__SECOND ;

int CPARAMETERS::NumInputs = __DEFAULT__NUM__INPUT ;
int CPARAMETERS::NumHidden = __DEFAULT__NUM__HIDDEN ;
int CPARAMETERS::NeuronsPerHiddenLayer = __DEFAULT__NEURONS__HIDDEN__LAYER ;
int CPARAMETERS::NumOutput = __DEFAULT__NUM__OUTPUT ;

double CPARAMETERS::ActivationResponse = __DEFAULT__ACTIVATION__RESPONSE ;
double CPARAMETERS::BIAS = __DEFAULT__BIAS ;

double CPARAMETERS::MaxTurnRate = __DEFAULT__TURNRATE__MAX ;
double CPARAMETERS::MaxSpeed = __DEFAULT__SPEED__MAX ;

int CPARAMETERS::SweeperScale = __DEFAULT__SCALE__SWEEPERS ;
int CPARAMETERS::MineScale = __DEFAULT__SCALE__MINE ;

int CPARAMETERS::NumSweepers = __DEFAULT__NUM__SWEEPERS ;
int CPARAMETERS::NumMines = __DEFAULT__NUM__MINES ;
int CPARAMETERS::NumMaxTicks = __DEFAULT__TICKS__MAX ;

double CPARAMETERS::CrossoverRate = __DEFAULT__CROSSOVER__RATE ;
double CPARAMETERS::MutationRate = __DEFAULT__MUTATION__RATE ;

double CPARAMETERS::MaxPerturbation = __DEFAULT__PERTURBATION__MAX ;
int CPARAMETERS::NumElites = __DEFAULT__NUM__ELITES ;
int CPARAMETERS::NumCopiesElite = __DEFAULT__NUM__COPIES__ELITE ;

const int PARAMETERASSIST::NumParameters = sizeof(ParametersName) / sizeof(ParametersName[0]) ;
void** PARAMETERASSIST::pParams = NULL ;
// Static member variables must be defined explicitly , or it won't have its own space .

CPARAMETERS::CPARAMETERS(void )
{
	// PARAMETERASSIST::InitilizeParamsPointer();

	if(!LoadedParaments("params.ini" ))
	{
		MessageBox(NULL ,"Failed to open params.ini" ,"Error" ,MB_OK );
	}
}

bool CPARAMETERS::LoadedParaments(const char* FileName )
{
	FILE* fin = NULL ;
	fopen_s(&fin ,FileName ,"r" );
	if(fin == NULL )return false ;
	size_t BufferSize = 0x1000 ;
	char* Buffer = (char*)malloc(BufferSize *sizeof(char));
	char* argptr = NULL ;
	while(!feof(fin ))
	{
		fgets(Buffer ,BufferSize ,fin );
		if(argptr = PARAMETERASSIST::ExistElement(Buffer ,'=' ))
		{
			*argptr = 0 ;
			argptr ++ ;
			if(*argptr )
			{
				void* ParamPtr = PARAMETERASSIST::FindParamAddress(Buffer );
				PARAMETERSTYPE type = PARAMETERASSIST::GetParameterType(ParamPtr );
				switch(type )
				{
					case type_int :
					{
						sscanf_s(argptr ,"%d" ,(int* )ParamPtr );
						break ;
					}
					case type_double :
					{
						sscanf_s(argptr ,"%lf" ,(double* )ParamPtr );
						break ;
					}
					default :
					{
						break ;
					}
				}
			}
		}
	}
	fclose(fin );
	return true ;
}


char* PARAMETERASSIST::ExistElement(char* ptr , char element )
{
	while(*ptr )
	{
		if(*ptr == element )return ptr ;
		ptr ++ ;
	}
	return NULL ;
}

void* PARAMETERASSIST::FindParamAddress(const char* ParamName )
{
	double Fitness[NumParameters] = {0};
	int best = 0 ;
	for(int i=0 ;i<NumParameters ;i++ )
	{
		Fitness[i] = 1.0 * LCS(ParamName ,ParametersName[i] ) / strlen(ParametersName[i] );
		if(Fitness[i] > Fitness[best] )best = i ;
	}
	if(Fitness[best] < 0.99 )
	{
		//throw(
		loger.printf("Expected Parameter Name : %s , received string : %s ,fitness : %lf \n",ParametersName[best] ,ParamName ,Fitness[best] );
	}
	return pParams[best] ;
}

PARAMETERASSIST::PARAMETERASSIST(void )
{
	pParams = (void** )malloc(NumParameters * sizeof(void*));
	int id = 0 ;
	pParams[id++] = &(CPARAMETERS::WindowWidth ) ;
	pParams[id++] = &(CPARAMETERS::WindowHeight) ;
	pParams[id++] = &(CPARAMETERS::FramesPerSecond ) ;
	pParams[id++] = &(CPARAMETERS::NumInputs ) ;
	pParams[id++] = &(CPARAMETERS::NumHidden ) ;
	pParams[id++] = &(CPARAMETERS::NeuronsPerHiddenLayer ) ;
	pParams[id++] = &(CPARAMETERS::NumOutput ) ;
	pParams[id++] = &(CPARAMETERS::ActivationResponse ) ;
	pParams[id++] = &(CPARAMETERS::BIAS ) ;
	pParams[id++] = &(CPARAMETERS::MaxTurnRate ) ;
	pParams[id++] = &(CPARAMETERS::MaxSpeed ) ;
	pParams[id++] = &(CPARAMETERS::SweeperScale ) ;
	pParams[id++] = &(CPARAMETERS::MineScale ) ;
	pParams[id++] = &(CPARAMETERS::NumSweepers ) ;
	pParams[id++] = &(CPARAMETERS::NumMines ) ;
	pParams[id++] = &(CPARAMETERS::NumMaxTicks ) ;
	pParams[id++] = &(CPARAMETERS::CrossoverRate ) ;
	pParams[id++] = &(CPARAMETERS::MutationRate ) ;
	pParams[id++] = &(CPARAMETERS::MaxPerturbation ) ;
	pParams[id++] = &(CPARAMETERS::NumElites ) ;
	pParams[id++] = &(CPARAMETERS::NumCopiesElite ) ;
}

PARAMETERSTYPE PARAMETERASSIST::GetParameterType(const void* ptr )
{
	for(int i=0 ;i<NumParameters ;i++ )
	{
		if(pParams[i] == ptr )
		{
			switch(i )
			{
				case 7 :case 8 :case 9 :case 10 :case 16 :case 17 :case 18 :
				{
					return type_double;
				}
				default :
				{
					return type_int ;
				}
			}
		}
	}
	return type_error ;
}

PARAMETERASSIST::~PARAMETERASSIST(void )
{
	free(pParams );
}

static char* __tmp__strupr(char* str )
{
	char* p = str ;
	while(*p )
	{
		if(*p >= 'a' && *p <= 'z' )*p -= 0x20 ;
		p++ ;
	}
	return str ;
}

static int LCS(const char* str1 ,const char* str2 )
{
	int l1 = strlen(str1 );
	int l2 = strlen(str2 );

	char* str1up = (char* )malloc((l1+1)*sizeof(char));
	memcpy(str1up ,str1 ,(l1+1)*sizeof(char) );
	__tmp__strupr(str1up );

	char* str2up = (char* )malloc((l2+1)*sizeof(char));
	memcpy(str2up ,str2 ,(l2+1)*sizeof(char) );
	__tmp__strupr(str2up );

	int* ans = (int*)calloc(l2 , sizeof(int));
	for(int i=0 ;i<l1 ;i++ )
	{
		for(int j=l2-1 ;j ;j-- )
		{
			if(str1up[i] == str2up[j] )
			{
				ans[j] = max(ans[j] ,ans[j-1]+1 );
			}
			else
			{
				ans[j] = max(ans[j] ,ans[j-1] );
			}
		}
		ans[0] = max(ans[0] ,str1up[i]==str2up[0]?1:0 );
	}
	int rans = ans[l2-1] ;
	free(ans );
	return rans ;
}