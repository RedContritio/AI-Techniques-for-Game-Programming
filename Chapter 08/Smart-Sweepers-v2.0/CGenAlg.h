#ifndef CGENALG__INC
#define CGENALG__INC
#pragma once

#include <vector>
using std::vector ;

struct GENOME
{
	vector<double> m_weights ;
	double m_fitness ;
	GENOME():m_fitness(0 ){}
	GENOME(vector<double> w ,double f ):m_weights(w ),m_fitness(f ){}
	friend bool operator<(const GENOME& lhs ,const GENOME& rhs )
	{
		return lhs.m_fitness <rhs.m_fitness ;
	}
};

class CGENALG
{
private :
	vector<GENOME> m_population ;
	vector<int> m_SplitPoints ;
	unsigned m_size ;
	int m_ChromoLength ;
	
	double m_BestFitness ;
	double m_AverageFitness ;
	double m_WorstFitness ;
	double m_TotalFitness ;

	int m_FittestGenome ;
	double m_MutationRate ;
	double m_CrossoverRate ;

	int m_Generation ;

	void Crossover(const vector<double>& parent1 ,const vector<double>& parent2 ,
				   vector<double>& child1 ,vector<double>& child2 );
	void Mutate(vector<double>& chromo );

	void GrabNBest(int NBest ,const int NumCopies ,vector<GENOME>& population);

	const GENOME& SelectParent(int N );

	void CalculateRelatedFitness(void );
	void Reset(void );

public :
	CGENALG(int size ,double Mrate ,double Xrate ,int NumWeights ,const vector<int>& splits );
	vector<GENOME> Epoch(vector<GENOME>& prevPop );
	vector<GENOME>& GetChromos(void ){return m_population ;}
	double AverageFitness(void )const {return m_AverageFitness ;}
	double BestFitness(void )const {return m_BestFitness ;}
};

#endif