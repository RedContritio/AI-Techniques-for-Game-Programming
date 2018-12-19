#include "Population.h"
#pragma warning(disable : 4018)

// #define __GENOME__CONSTRUCTOR__LPRINT
GENOME::GENOME(int length )
{
	const int ops[] = {GAME_LEFT ,GAME_UP ,GAME_RIGHT ,GAME_DOWN };
	for(int i=0 ;i<length ;i++ )
	{
		vecOperations.push_back(ops[RandInt(0 ,4-1 )]);
	}
#ifdef __GENOME__CONSTRUCTOR__LPRINT
	GenomeToLog(vecOperations );
#endif
	fitness = 0 ;
}

GENOME::~GENOME(void )
{
	;
}

double GENOME::Fitness(void )const 
{
	return fitness ;
}

vector<int> GENOME::GetOperations(void )const 
{
	return vecOperations ;
}

double GENOME::CalculateFitness(const GAME &game )
{
	GAME tmp = game ;
	for(int i=0 ;i<vecOperations.size() ;i++ )
	{
		tmp.Move(vecOperations[i] );
	}
	tmp.CalculateFitness( );
	return fitness = tmp.Fitness() ;
}

void GenomeToLog(const vector<int>& ops )
{
	static int diyicitiaozhuan[256] = {0 };
	diyicitiaozhuan[GAME_LEFT ] = 0 ;
	diyicitiaozhuan[GAME_UP ] = 1 ;
	diyicitiaozhuan[GAME_RIGHT ] = 2 ;
	diyicitiaozhuan[GAME_DOWN ] = 3 ;
	static const int SecondDirect[] = {'L' ,'U' ,'R' ,'D' };
	char strops[256] = "" ;
	int soslen = 0 ;
	for(int i=0 ;i<ops.size() ;i++ )
	{
		strops[i] = SecondDirect[diyicitiaozhuan[ops[i]]] ;
	}
	loger.lprint("genome ops : %s\n" ,strops );
}