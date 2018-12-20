#include "GeneticAlgorithm.h"
#include <algorithm>
#include "LogSpawner.h"
using RedContritio::LogSpawner ;
extern LogSpawner loger ;
#pragma warning(push )
#pragma warning(disable:4018)
static void MutateCM(vector<double>& chromo ,double mRate );
static void CrossoverEC(const vector<double>& ,const vector<double>& ,vector<double>& ,vector<double>& ,double );

static const int LowLimitOfScore =9999999 ;
CGenAlg::CGenAlg(int popsize ,double MutationRate ,double CrossoverRate ,int NumWeights ,vector<int>& splits ):
m_PopulationSize(popsize ),m_MutationRate(MutationRate ),m_CrossoverRate(CrossoverRate ),
m_ChromoLength(NumWeights ),m_Generation(0 ),m_FitnessGenome(0 ),m_SplitPoints(splits )
{
	Reset();
	for(int i=0 ;i<m_PopulationSize ;i++ )
	{
		m_Population.push_back(GENOME());
		for(int j=0 ;j<m_ChromoLength ;j++ )
		{
			m_Population[i].m_weights.push_back(RandomClamped());
		}
	}
	Mutate =MutateCM ;
	Crossover =CrossoverEC ;
}

vector<GENOME> CGenAlg::Epoch(vector<GENOME>& prevPopulation)
{
	m_Population =prevPopulation ;

	Reset();
	std::sort(m_Population.begin() ,m_Population.end());
	CalculateRelatedFitness();

	vector<GENOME> NewPopulation ;
	if(CParam::NumOfCopiesElite *CParam::NumOfElite %2 == 0 ) // couples of elites
	{
		GrabNBest(CParam::NumOfElite ,CParam::NumOfCopiesElite ,NewPopulation );
	}

	while(NewPopulation.size() <m_PopulationSize )
	{
		GENOME parent1 =GetChromoRoulette();
		GENOME parent2 =GetChromoRoulette();
		
		vector<double> child1 ,child2 ;

		CrossoverAtSplits(parent1.m_weights ,parent2.m_weights ,child1 ,child2 ,m_CrossoverRate );

		Mutate(child1 ,m_MutationRate );
		Mutate(child2 ,m_MutationRate );

		NewPopulation.push_back(GENOME(child1 ,0 ));
		NewPopulation.push_back(GENOME(child2 ,0 ));
	}
	m_Population =NewPopulation ;
	return m_Population ;
}

void CGenAlg::CrossoverAtSplits(const vector<double>& parent1 ,const vector<double>& parent2 ,
								vector<double>& child1 ,vector<double>& child2 , double rate )
{
	if(RandFloat() >rate || parent1 == parent2 )
	{
		child1 =parent1 ;

		child2 =parent2 ;
		return ;
	}
	
	int tcp1 =RandInt(0, m_SplitPoints.size()-2) ;
	int tcp2 =RandInt(tcp1, m_SplitPoints.size()-1);
	int cp1 = m_SplitPoints[tcp1 ];
	int cp2 = m_SplitPoints[tcp2 ];

	for(int i=0 ;i<parent1.size() ;i++ )
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

GENOME CGenAlg::GetChromoRoulette(void)
{
	double Slice =RandFloat() *m_TotalFitness ;
	GENOME& ChosenGenome =m_Population[0] ;
	double Plus =0 ;
	for(int i=0 ;i<m_PopulationSize ;i++ )
	{
		Plus += m_Population[i].m_Fitness ;
		if(Plus >= Slice )
		{
			ChosenGenome =m_Population[i] ;
			break ;
		}
	}
	return ChosenGenome ;
}

void CGenAlg::GrabNBest(int NBest, const int NumCopies, vector<GENOME>& population)
{
	while(NBest-- )
	{
		for(int i=0 ;i<NumCopies ;i++ )
		{
			population.push_back(m_Population[m_PopulationSize-1-NBest]);
		}
	}
	return ;
}

void CGenAlg::CalculateRelatedFitness(void)
{
	m_TotalFitness =0 ;
	double HighestFitness =0 ,LowestFitness =LowLimitOfScore ;

	for(int i=0 ;i<m_PopulationSize ;i++ )
	{
		m_TotalFitness += m_Population[i].m_Fitness ;
		if(m_Population[i].m_Fitness >HighestFitness )
		{
			HighestFitness =m_Population[i].m_Fitness ;
			m_FitnessGenome =i ;
			m_BestFitness =HighestFitness ;
		}
		if(m_Population[i].m_Fitness <LowestFitness )
		{
			LowestFitness =m_Population[i].m_Fitness ;
			m_WorstFitness =LowestFitness ;
		}
	}

	m_AverageFitness =m_TotalFitness /m_PopulationSize ;
}

void CGenAlg::Reset(void )
{
	m_TotalFitness =0 ;
	m_BestFitness =0 ;
	m_WorstFitness =LowLimitOfScore ;
	m_AverageFitness =0 ;
}


static void MutateCM(vector<double>& chromo ,double mRate )
{
	for(size_t i=0 ;i<chromo.size() ;i++ )
	{
		if(RandFloat() <mRate )
		{
			chromo[i] += RandomClamped() *CParam::dMaxPerturbation ;
		}
	}
}

void CrossoverEC(const vector<double>& parent1 ,const vector<double>& parent2 ,
				 vector<double>& child1 ,vector<double>& child2 , double rate )
{
	if(RandFloat() >rate || (parent1 ==parent2 ))
	{
		child1 =parent1 ;
		child2 =parent2 ;
		return ;
	}

	int point =RandInt(0 ,parent1.size()-1 );
	for(int i=0 ;i<point ;i++ )
	{
		child1.push_back(parent1[i] );
		child2.push_back(parent2[i] );
	}
	for(int i=point ;i<parent1.size() ;i++ )
	{
		child1.push_back(parent2[i] );
		child2.push_back(parent1[i] );
	}
	return ;
}

#pragma warning(pop )