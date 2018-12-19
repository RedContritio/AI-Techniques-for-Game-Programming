#ifndef __GAME__INC
#define __GAME__INC

#include <windows.h>
#include <algorithm>
#include <vector>
#include "LogSpawner.h"
#include "defines.h"
#include "utils.h"

using std::swap ;
using std::vector ;
using RedContritio::LogSpawner ;

extern int cxClient ,cyClient ;
extern LogSpawner loger ;
class GAME
{
	private :
		int map[9] ;
		int pos ;
		double _Fitness ;
		void VoidLocatation(void );
		void SpawnRandomMap(void );
		bool CheckSolvable(void );
	public :
		void CalculateFitness(void );
		GAME(void );
		GAME(int );
		~GAME(void );
		double Fitness(void )const ;
		int* GetMap(void ) ;
		bool Move(int operation );
		double BestFitness(void )const ;
};
GAME& ApplyOperations(GAME &game ,const vector<int> &ops );
void GameRenderer(HDC ,GAME & );
#endif