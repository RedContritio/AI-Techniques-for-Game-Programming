#ifndef __GENOME__INC
#define __GENOME__INC
#include <windows.h>
#include <vector>
#include "Game.h"
#include "defines.h"
#include "utils.h"

using std::vector ;

class GENOME
{
	private :
		vector<int> vecOperations ;
		double fitness ;
	public :
		GENOME(int );
		~GENOME(void );
		void CalculateFitness(const GAME & );
		double Fitness(void )const ;
		friend class POPULATION ;
		friend void LoadOperation(const GAME & ,const GENOME & );
		friend bool ShowOperation(HDC );
};

class POPULATION
{
	private :
		vector<GENOME> vecGenomes ;
		GAME *pGame ;
		double MutateRate ;
		double CrossoverRate ;
		int PopulationSize ;
		int ChromosomeLength ;

		bool isStarted ;

		double BestFitness ;
<<<<<<< HEAD
		double TotalFitness ;

		int FittestGenome ;
		int Generation ;

		GENOME& RouletteSelection(void );

		void Mutate(vector<int> & );

		void Crossover(const vector<int> & ,const vector<int> & ,
					   vector<int> & ,vector<int> & );

		void CalculatePopulationFitness(void );
		void GenerateInitialGeneration(void );
		void Reset(void );
=======
		GENOME& Roulette
>>>>>>> parent of 8af1774... Update
	public :
		bool SlowMode ;
		POPULATION(double ,double ,int ,int );
		~POPULATION();
		void Epoch(void );
		void Render(HDC );
		void Run(void );
		void Stop(void );
		bool Started(void );
		GENOME GetFittestGenome(void );
		GAME* GetGame(void );
};

void MutateSM(vector<int> & );

void CrossoverDPX(const vector<int> & ,const vector<int> & ,
				  vector<int> & ,vector<int> & );

extern GAME SW_GAME ;
extern vector<int> SW_OPS ;
extern int SW_i ;
extern bool SW_Showing ;
void LoadOperation(const GAME & ,const GENOME & );
bool ShowOperation(HDC );
#endif
