#ifndef __GENOME__DEF
#define __GENOME__DEF

#include <algorithm>
#include <vector>
#include <windows.h>
#include "Map.h"
#include "utils.h"
using std::vector ;
using std::swap ;
using std::find ;
class GENOME
{
	private :
		vector<int> vecRoute ;
		double Fitness ;
	public :
		GENOME(void );
		GENOME(int );
		vector<int> GrabPermutation(int & );
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
		mutable int Generation ;
		bool isStarted ;

		void MutateEM(vector<int> & ); // Exchange Mutation
		void CrossoverPMX(const vector<int> & ,const vector<int> & ,
						  vector<int> & ,vector<int> & ); // Partially Mapped Crossover
		GENOME& RouletteWheelSelection(void );

		void CalculatePopulationFitness(void );
		
		void Reset(void );
		void CreateInitialGeneration(void );
	public :
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