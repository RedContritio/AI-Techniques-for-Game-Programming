#ifndef __POPULATION__DEF
#define __POPULATION__DEF
#include <windows.h>
#include <vector>
#include "Game.h"
#include "defines.h"
#include "LogSpawner.h"

using RedContritio::LogSpawner ;
using std::vector ;

extern LogSpawner loger ;

class GENOME
{
	private :
		vector<int> vecOperations ;
		double fitness ;
	public :
		GENOME(int );
		~GENOME(void );
		double Fitness(void )const ;
		vector<int> GetOperations(void )const ;
		double CalculateFitness(const GAME & );
		friend class POPULATION ;
};

class POPULATION
{
	private :
		vector<GENOME> vecGenomes ;
		GAME game ;
		double mutateRate ;
		double crossoverRate ;
		int populationSize ;
		int chromosomeLength ;

		double HighestFitness ;
		double TotalFitness ;
		int FittestGenome ;

		int Generation ;
		bool isStarted ;

		GENOME& RouletteSelection(void );

		void Crossover(const vector<int> & ,const vector<int> & ,
					   vector<int> & ,vector<int> & );

		void Mutate(vector<int> & );
		void Reset(void );
		void CalculateFitness(void );

		void GreateInitialUnits(void );
	public :
		POPULATION(double ,double ,int ,int );
		~POPULATION(void );

		void Epoch(void );
		void Render(HDC );
		void Run(void );
		void Stop(void );

		void EliteToLog(void );
		bool Started(void );
		GENOME& Elite(void );
		GAME& GameBoard(void );
};

void ClearUselessOperations(const GAME & ,vector<int> & );
void ScrambleMutate(vector<int> & );
void NormalMutate(vector<int> & );
void GenomeToLog(const vector<int> & );
#endif