#include "CGenAlg.h"
#include "CParam.h"
#include "utils.h"
#include <algorithm>

static const double ValueOfMaxFitness =1e15 ;
CGENALG::CGENALG(int size, double Mrate, double Xrate, int NumWeights, const vector<int>& splits):
m_size(size ),m_MutationRate(Mrate ),m_CrossoverRate(Xrate ),m_ChromoLength(NumWeights ),m_SplitPoints(splits ),
m_Generation(0 )
{
	Reset();
	for(size_t i=0 ;i<m_size ;i++ )
	{
		m_population.push_back(GENOME());
		for(int j=0 ;j<m_ChromoLength ;j++ )
		{
			m_population[i].m_weights.push_back(RandClamped());
		}
	}
}

vector<GENOME> CGENALG::Epoch(vector<GENOME>& prevPop)
{
	m_population =prevPop ;
	Reset();

	vector<GENOME> nextPop ;

	CalculateRelatedFitness();

	std::sort(m_population.begin() ,m_population.end());

	if((CPARAM::NumCopiesElite *CPARAM::NumElite )%2 ==0 )
	{
		GrabNBest(CPARAM::NumElite ,CPARAM::NumCopiesElite ,nextPop );
	}

	while(nextPop.size() <m_size )
	{
		const GENOME& parent1 =SelectParent(4 );
		const GENOME& parent2 =SelectParent(4 );

		vector<double> child1 ,child2 ;

		Crossover(parent1.m_weights ,parent2.m_weights ,child1 ,child2 );

		Mutate(child1 );
		Mutate(child2 );

		nextPop.push_back(GENOME(child1 ,0 ));
		nextPop.push_back(GENOME(child2 ,0 ));
	}

	m_population =nextPop ;
	return m_population ;
}

void CGENALG::Mutate(vector<double>& chromo )
{
	for(size_t i=0 ;i<chromo.size() ;i++ )
	{
		if(RandFloat() <m_MutationRate )
		{
			chromo[i] += RandClamped() *CPARAM::MaxPerturbation ;
		}
	}
}

void CGENALG::GrabNBest(int NBest, const int NumCopies, vector<GENOME>& population)
{
	while(NBest-- )
	{
		for(int i=0 ;i<NumCopies ;i++ )
		{
			population.push_back(m_population[m_size-1-NBest]);
		}
	}
}

void CGENALG::Crossover(const vector<double>& parent1 ,const vector<double>& parent2 ,
						vector<double>& child1 ,vector<double>& child2 )
{
	if(RandFloat() >m_CrossoverRate || parent1 ==parent2 )
	{
		child1 =parent1 ;
		child2 =parent2 ;
		return ;
	}
	
	int index1 =RandInt(0 ,m_SplitPoints.size()-1);
	int index2 =RandInt(index1 ,m_SplitPoints.size());

	size_t cp1 =m_SplitPoints[index1 ];
	size_t cp2 =m_SplitPoints[index2 ];

	for(size_t i=0 ;i<parent1.size() ;i++ )
	{
		if((i<cp1 )||(i>=cp2 ))
		{
			child1.push_back(parent1[i] );
			child2.push_back(parent2[i] );
		}
		else
		{
			child1.push_back(parent2[i] );
			child2.push_back(parent1[i] );
		}
	}

	return ;
}

const GENOME& CGENALG::SelectParent(int N )
{
	double BestFitness =-ValueOfMaxFitness ;
	int fi =0 ;

	for(int i=0 ;i<N ;i++ )
	{
		int tp =RandInt(0 ,m_size );
		if(m_population[tp].m_fitness >BestFitness )
		{
			fi =tp ;
			BestFitness =m_population[tp].m_fitness ;
		}
	}

	return m_population[fi];
}

void CGENALG::CalculateRelatedFitness(void)
{
	Reset();

	for(size_t i=0 ;i<m_size ;i++ )
	{
		if(m_population[i].m_fitness >m_BestFitness )
		{
			m_BestFitness =m_population[i].m_fitness ;
			m_FittestGenome =i ;
		}
		if(m_population[i].m_fitness <m_WorstFitness )
		{
			m_WorstFitness =m_population[i].m_fitness ;
		}
		m_TotalFitness += m_population[i].m_fitness ;
	}
	m_AverageFitness =m_TotalFitness /m_size ;
}

void CGENALG::Reset(void)
{
	m_BestFitness =-ValueOfMaxFitness ;
	m_WorstFitness =ValueOfMaxFitness ;
	m_TotalFitness =0 ;
	m_AverageFitness =0 ;
}