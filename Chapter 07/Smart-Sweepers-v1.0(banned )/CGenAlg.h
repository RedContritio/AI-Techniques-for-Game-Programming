#ifndef __GENALG__DEF
#define __GENALG__DEF
#pragma once
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "utils.hpp"
#include "CParams.h"

using std::vector ;
using std::sort ;

struct SGENOME
{
	vector<double> Weights ;
	double Fitness ;

	SGENOME(void ):Fitness(0 ){}
	SGENOME(vector<double> w ,double f ):Weights(w ),Fitness(f ){}

	friend bool operator<(const SGENOME& lhs ,const SGENOME& rhs )
	{
		return lhs.Fitness < rhs.Fitness ;
	}
};

class CGeneticAlgorithm
{
	private:
		vector<SGENOME> vecPop ;
		int PopulationSize ;
		int ChromoLength ;
		
		double mTotalFitness ;
		double mBestFitness ;
		double mAverageFitness ;
		double mWorstFitness ;

		int FittestGenome ;

		double MutationRate ;
		double CrossoverRate ;

		int Generation ;

		void Crossover(const vector<double>& ,const vector<double>& ,
					   vector<double>& ,vector<double>& );
		void Mutate(vector<double>& );
		SGENOME GetGhromoParent(void );
		void GrabNBest(int ,const int ,vector<SGENOME>& );

		void FitnessScaleRank(void );
		void CalculateRelatedFitness(void );

		void Reset(void );

	public :
		CGeneticAlgorithm(int ,double ,double ,int );
		vector<SGENOME> Epoch(vector<SGENOME> &population );

		vector<SGENOME> GetChromos(void )const {return vecPop ;}
		double AverageFitness(void )const {return mTotalFitness / PopulationSize ;}
		double BestFitness(void )const {return mBestFitness ;}
};

#endif
