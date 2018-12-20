#include "CGenAlg.h"
CGeneticAlgorithm::CGeneticAlgorithm(int popsize ,double crossoverrate ,double mutationrate ,int numweights )
:PopulationSize(popsize ),CrossoverRate(crossoverrate ),MutationRate(mutationrate ),ChromoLength(numweights ),
 mTotalFitness(0 ),Generation(0 ),FittestGenome(0 ),mBestFitness(0 ),mWorstFitness(99999999 ),mAverageFitness(0 )
{
	for(int i=0 ;i<PopulationSize ;i++ )
	{
		vecPop.push_back(SGENOME());
		for(int j=0 ;j<ChromoLength ;j++ )
		{
			vecPop[i].Weights.push_back(RandClamped());
		}
	}
}

vector<SGENOME> CGeneticAlgorithm::Epoch(vector<SGENOME> &PrevPop )
{
	vecPop = PrevPop ;

	Reset();

	sort(vecPop.begin() ,vecPop.end());

	CalculateRelatedFitness();

	vector<SGENOME> NextPop ;

	if(!(CPARAMETERS::NumCopiesElite * CPARAMETERS::NumElites % 2 ))
	{
		GrabNBest(CPARAMETERS::NumElites ,CPARAMETERS::NumCopiesElite ,NextPop );
	}

	while(NextPop.size() < (size_t)PopulationSize )
	{
		SGENOME parent1 = GetGhromoParent();
		SGENOME parent2 = GetGhromoParent();

		vector<double> child1 ,child2 ;

		Crossover(parent1.Weights ,parent2.Weights ,child1 ,child2 );

		Mutate(child1 );
		Mutate(child2 );

		NextPop.push_back(SGENOME(child1 ,0 ));
		NextPop.push_back(SGENOME(child2 ,0 ));
	}

	vecPop = NextPop ;
	return vecPop ;
}

void CGeneticAlgorithm::Crossover(const vector<double>& parent1 ,const vector<double>& parent2 ,
								  vector<double>& child1 ,vector<double>& child2 )
{
	if((RandFloat() > CrossoverRate) || (parent1 == parent2 ))
	{
		child1 = parent1 ;
		child2 = parent2 ;
		return ;
	}

	int cp = RandInt(0 ,ChromoLength-1 ); // Cross Point
	for(int i=0 ;i<cp ;i++ )
	{
		child1.push_back(parent1[i] );
		child2.push_back(parent2[i] );
	}
	for(int i=cp ;i<ChromoLength ;i++ )
	{
		child1.push_back(parent2[i] );
		child2.push_back(parent1[i] );
	}
	return ;
}

void CGeneticAlgorithm::Mutate(vector<double>& chromo )
{
	for(int i=0 ;i<ChromoLength ;i++ )
	{
		if(RandFloat() < MutationRate )
		{
			chromo[i] += RandClamped() * CPARAMETERS::MaxPerturbation ;
		}
	}
	return ;
}

SGENOME CGeneticAlgorithm::GetGhromoParent(void )
{
	double Slice = RandFloat() * mTotalFitness ;
	double NowFitness = 0 ;
	for(int i=0 ;i<PopulationSize ;i++ )
	{
		NowFitness += vecPop[i].Fitness ;
		if(NowFitness >= Slice )
		{
			return vecPop[i] ;
		}
	}
	return vecPop[PopulationSize-1] ;
}

void CGeneticAlgorithm::GrabNBest(int NBest ,const int NumCopies ,vector<SGENOME>& Population )
{
	while(NBest -- )
	{
		for(int i=0 ;i<NumCopies ;i++ )
		{
			Population.push_back(vecPop[(PopulationSize-1) - NBest ]);
		}
	}
}

void CGeneticAlgorithm::FitnessScaleRank(void )
{
	const int FitnessMultiplier = 1 ;

	for(int i=0 ;i<PopulationSize ;i++ )
	{
		vecPop[i].Fitness = i * FitnessMultiplier ;
	}

	CalculateRelatedFitness();
}

void CGeneticAlgorithm::CalculateRelatedFitness(void )
{
	Reset();

	for(int i=0 ;i<PopulationSize ;i++ )
	{
		if(vecPop[i].Fitness > mBestFitness )
		{
			mBestFitness = vecPop[i].Fitness ;
			FittestGenome = i ;
		}
		if(vecPop[i].Fitness < mWorstFitness )
		{
			mWorstFitness = vecPop[i].Fitness ;
		}
		mTotalFitness += vecPop[i].Fitness ;
	}

	mAverageFitness = mTotalFitness / PopulationSize ;
}

void CGeneticAlgorithm::Reset(void )
{
	mTotalFitness = 0 ;
	mBestFitness = 0 ;
	mWorstFitness = 99999999 ;
	mAverageFitness = 0 ;
}