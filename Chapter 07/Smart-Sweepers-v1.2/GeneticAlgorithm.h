#ifndef __GENETIC_ALGORITHM__INC
#define __GENETIC_ALGORITHM__INC
#pragma once
#include <vector>
#include "utils.h"
#include "CParam.h"

using std::vector ;

struct GENOME
{
	vector<double> m_weights ;
	double m_Fitness ;
	GENOME():m_Fitness(0 ){}
	GENOME(vector<double>& v ,double f ):m_weights(v ),m_Fitness(f ){}

	friend bool operator<(const GENOME& lhs ,const GENOME& rhs )
	{
		return (lhs.m_Fitness <rhs.m_Fitness );
	}
};

class CGenAlg
{
private :
	vector<GENOME> m_Population ;
	int m_PopulationSize ;
	int m_ChromoLength ;
	vector<int> m_SplitPoints ;
	double m_TotalFitness ;
	double m_BestFitness ;
	double m_AverageFitness ;
	double m_WorstFitness ;
	int m_FitnessGenome ;
	double m_MutationRate ;
	double m_CrossoverRate ;
	int m_Generation ;

	void (*Crossover )(const vector<double>& ,const vector<double>& ,vector<double>& ,vector<double>& ,double );
	void CrossoverAtSplits(const vector<double>& ,const vector<double>& ,vector<double>& ,vector<double>& ,double );
	void (*Mutate )(vector<double>& ,double );
	GENOME GetChromoRoulette(void );
	void GrabNBest(int NBest ,const int NumCopies ,vector<GENOME>& population );
	void FitnessScaleRank(void );
	void CalculateRelatedFitness(void );
	void Reset(void );

public :
	CGenAlg(int ,double ,double ,int ,vector<int>& );
	vector<GENOME> Epoch(vector<GENOME>& prevPopulation );
	vector<GENOME> GetChromos(void )const {return m_Population ;}
	double AverageFitness(void )const {return m_TotalFitness /m_PopulationSize ;}
	double BestFitness(void )const {return m_BestFitness ;}
};

#endif