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
#include "LogSpawner.h"

using RedContritio::LogSpawner ;
extern LogSpawner loger ;
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

		void (*Mutate )(vector<int> & );
		void (*Crossover )(const vector<int> & ,const vector<int> & ,
					   vector<int> & ,vector<int> & );
		GENOME& (*SelectParent)(vector<GENOME> & ,double );

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

		void SelectMutationType(int );
		void SelectCrossoverType(int );
};

void MutateEM(vector<int> & ); // Exchange Mutation 
void MutateDM(vector<int> & ); // Displacement Mutation 
void MutateSM(vector<int> & ); // Scramble Mutation 
void MutateIM(vector<int> & ); // Insert Mutation 
void MutateAM(vector<int> & ); // Automatically Switch Mutation

void CrossoverOBX(const vector<int> & ,const vector<int> & ,
				   vector<int> & ,vector<int> & );
void CrossoverPBX(const vector<int> & ,const vector<int> & ,
				   vector<int> & ,vector<int> & );
void CrossoverPMX(const vector<int> & ,const vector<int> & ,
				   vector<int> & ,vector<int> & );

void SectionChoose(int & ,int & ,int ,int );

GENOME& RouletteWheelSelection(vector<GENOME> & ,double );
#endif