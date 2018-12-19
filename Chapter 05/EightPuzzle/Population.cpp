#include "Population.h"
#pragma warning( disable:4996)
static int SamePopulation ;
static double PrevFitness ;
POPULATION::POPULATION( double MutateRate ,double CrossoverRate ,int PopulationSize ,int SeriesLength )
:mutateRate(MutateRate ),crossoverRate(CrossoverRate ),populationSize(PopulationSize ),chromosomeLength(SeriesLength),
isStarted(false ),Generation(0 ),HighestFitness(0 ),FittestGenome(0 )
{
	;
}

POPULATION::~POPULATION(void)
{
	;
}

void POPULATION::GreateInitialUnits(void)
{
	for(int i=0 ;i<populationSize ;i++)
	{
		vecGenomes.push_back(GENOME(chromosomeLength ));
	}
	Generation = 0 ;
}

bool POPULATION::Started(void )
{
	return isStarted ;
}

void POPULATION::CalculateFitness(void )
{
	HighestFitness = 0 ;
	TotalFitness = 0 ;
	FittestGenome = 0 ;
	for(int i=0 ;i<populationSize ;i++ )
	{
		vecGenomes[i].CalculateFitness(game ) ;
		TotalFitness += vecGenomes[i].fitness ;
		if(vecGenomes[i].fitness < game.BestFitness()-EPSILON )
		{
			if((ApplyOperations(game ,vecGenomes[i].vecOperations ).GetMap())[0] == 0 )
			{
				vecGenomes[i].fitness -= 0.1*SamePopulation ;
			}
		}
		if(vecGenomes[i].fitness > HighestFitness )
		{
			FittestGenome = i ;
			HighestFitness = vecGenomes[i].Fitness( );
		}
	}
}

void POPULATION::Epoch(void )
{
	Reset();
	CalculateFitness( );
	if(PrevFitness >= HighestFitness )
	{
		SamePopulation ++ ;
	}
	else
	{
		SamePopulation = 0 ;
		PrevFitness = HighestFitness ;
	}

	loger.lprint("Generation : %d ; Fittest : %lf%%\n" ,Generation ,100 * HighestFitness / (game.BestFitness()));
	EliteToLog();

	vector<GENOME> NextPopu ;
	int i ; // fuck VC++
	for(i=0 ;i<__NUM__TO__INHERIT ;i++ )
	{
		NextPopu.push_back(vecGenomes[FittestGenome]);
	}
	/*
	for(i=0 ;i<__NUM__TO__DISTURB ;i++ )
	{
		NextPopu.push_back(GENOME(chromosomeLength));
	}*/
	while(NextPopu.size() < populationSize )
	{
		GENOME parent1 = RouletteSelection() ;
		GENOME parent2 = RouletteSelection() ;
		GENOME child1 = parent1 ;
		GENOME child2 = parent2 ;

		if(RandFloat() < __XOVER__RATE )
		{
			Crossover(parent1.vecOperations ,parent2.vecOperations ,
					  child1.vecOperations ,child2.vecOperations );
		}

		if(RandFloat() < __MUTATION__RATE )Mutate(child1.vecOperations );
		if(RandFloat() < __MUTATION__RATE )Mutate(child2.vecOperations );

		NextPopu.push_back(child1 );
		NextPopu.push_back(child2 );
	}
	vecGenomes = NextPopu ;
	Generation ++ ;
	return ;
}

void POPULATION::Mutate(vector<int> &genome )
{
	if( RandFloat() < 0.01 )
	{
		ClearUselessOperations(game ,genome );
	}
	else if(Generation / 10 % 2 )
	{
		ScrambleMutate(genome );
	}
	else
	{
		NormalMutate(genome );
	}
}

void POPULATION::Crossover(const vector<int> &parent1 ,const vector<int> &parent2 ,
						   vector<int> &child1 ,vector<int> &child2 )
{
	int p1 = RandInt(0 ,parent1.size() - 1 );
	int p2 = RandInt(0 ,parent1.size() - 1 );
	if(p1 > p2 )swap(p1 ,p2 );
	for(int i=p1 ;i<=p2 ;i++ )
	{
		child1[i] = parent2[i] ;
		child2[i] = parent1[i] ;
	}
	return ;
}

GENOME& POPULATION::RouletteSelection(void )
{
	double nFitness = RandFloat() * TotalFitness ;
	double tFitness = 0 ;
	for(int i=0 ;i<populationSize ;i++ )
	{
		tFitness += vecGenomes[i].fitness ;
		if(tFitness > nFitness )return vecGenomes[i] ;
	}
	return vecGenomes[populationSize-1];
}

void POPULATION::Reset(void )
{
	HighestFitness = 0 ;
	FittestGenome = 0 ;
	TotalFitness = 0 ;
}

void POPULATION::Run(void )
{
	GreateInitialUnits( );
	isStarted = true ;
}

void POPULATION::Stop(void )
{
	isStarted = false ;
}

void POPULATION::EliteToLog(void )
{
	loger.lprint("Fittest %d : %lf %lf%%\n",FittestGenome ,vecGenomes[FittestGenome].fitness ,
							100*(vecGenomes[FittestGenome].fitness)/(game.BestFitness()) );
	GenomeToLog(vecGenomes[FittestGenome].vecOperations );
}

GENOME& POPULATION::Elite(void )
{
	return vecGenomes[FittestGenome];
}

GAME& POPULATION::GameBoard(void )
{
	return game;
}

void POPULATION::Render(HDC hdc )
{
	if(Started() )
	{
		GENOME tmpGenome = Elite();
		GAME tmpGame = game;
		GameRenderer(hdc ,ApplyOperations(tmpGame ,tmpGenome.GetOperations() ));
		char strGeneration[20] = "" ;
		sprintf(strGeneration ,"Generation : %d" ,Generation );
		TextOut(hdc ,5 ,5 ,strGeneration ,strlen(strGeneration ));
	}
	else
	{
		GameRenderer(hdc ,game );
	}
}

void ClearUselessOperations(const GAME &game ,vector<int> &genome )
{
	const int ops[] = {GAME_LEFT ,GAME_UP ,GAME_RIGHT ,GAME_DOWN };
	GAME tmp = game ;
	int chromosomeLength = genome.size() ;
	int i=0 ; // fuck VC++
	for(i=0 ;i<genome.size() ;i++ )
	{
		if(!(tmp.Move(genome[i])))
		{
			genome.erase(genome.begin()+i);
			i-- ;
		}
	}
	for(i=genome.size() ;i<chromosomeLength ;i++ )
	{
		genome.push_back(ops[RandInt(0 ,4-1 )]);
	}
}

void ScrambleMutate(vector<int> &genome )
{
	const int MinLength = 4 ;
	int p1 = RandInt(0 ,genome.size() - MinLength );
	int p2 = RandInt(p1+MinLength-1 ,genome.size()-1 );
	for(int i=0 ;i<p2-p1 ;i++ )
	{
		int p3 = RandInt(p1 ,p2 );
		int p4 = RandInt(p1 ,p2 );
		swap(genome[p3] ,genome[p4] );
	}
}

void NormalMutate(vector<int> &genome )
{
	const int ops[] = {GAME_LEFT ,GAME_UP ,GAME_RIGHT ,GAME_DOWN };
	for(int i=0 ;i<RandInt(1 ,genome.size()/4 );i++ )
	{
		int p = RandInt(0 ,genome.size()-1 );
		genome[p] = ops[RandInt(0 ,4-1 )] ;
	}
}