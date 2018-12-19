#ifndef __POPULATION__DEF
#define __POPULATION__DEF
#include "defines.h"
#include <windows.h>
#include <algorithm>
#include <vector>

using std::vector ;
using std::swap ;

#include "map.h"
#include "utils.h"

void RouteRenderer(HDC ,const MAP & ,const vector<int> & );

class GENOME
{
	private :
		vector<int> vecCities ;
		double _Fitness ;
		void GeneratePermutation(int );
	public :
		GENOME(void );
		GENOME(int );
		double Fitness(void );
		vector<int> GetRoute(void );
		friend void RouteRenderer(HDC ,const MAP & ,const vector<int> & );
		friend class POPULATION ;
};

class POPULATION
{
	private :
		vector<GENOME> vecGenomes ;
		MAP *pMap ;
		double MutationRate ;
		double CrossoverRate ;
		double TotalFitness ;
		double ShortestDistance ;
		double LongestDistance ;
		int PopulationSize ;
		int ChromosomeLength ;
		int FittestGenome ;
		int Generation ;
		bool isStarted ;

		void Mutate(vector<int> & );
		void Crossover(const vector<int> & ,const vector<int> & ,
					   vector<int> & ,vector<int> & );
		GENOME& RouletteWheelSelection(void );

		void CalculatePopulationFitness(void );
		void GenerateInitialGeneration(void );

		void Reset(void );
	public:
		POPULATION(double ,double ,int ,int ,int ,int );
		~POPULATION(void );
		void Epoch(void );
		void Run(HWND );
		void Render(HDC ,int ,int );
		void Resize(int ,int );
		void Stop(void );
		bool Started(void );
};

#endif