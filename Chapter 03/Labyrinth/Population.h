#ifndef __PLAYER__DEF
#define __PLAYER__DEF
#include "Map.h"
#include <vector>
using std::vector ;
struct GENOME
{
	vector<int> vecBits ;
	double Fitness ;
	GENOME(void );
	GENOME(const int );
};

class POPULATION
{
	private :
		vector<GENOME> vecGenome ;
		int PopSize ;
		double CrossoverRate ;
		double MutationRate ;
		
		int ChromosomeLength ;
		int GeneLength ;
		int FittestGenome ;
		double BestFitnessScore ;
		double TotalFitnessScore ;
		int Generation ;

		MAP Map ;
		MAP Brain ;

		bool isBusy ;
		
		void Mutate(vector<int> & );
		void Crossover(const vector<int> & ,const vector<int> & ,
					   vector<int> & ,vector<int> & );
		GENOME& RouletteWheelSelection(void );

		void UpdateFitnessScore(void );
		vector<int>	Decode(const vector<int> & );
		int BinToInt(const vector<int> & );

		void CreateInitialPopulation(void );
	public :
		POPULATION(double ,double ,int ,int ,int );
		void Run(HWND );
		void Render(int ,int ,HDC );
		void Epoch(void );

		int GetGeneration(void );
		int GetFittest(void );
		bool Started(void );
		void Stop(void );
};
#endif