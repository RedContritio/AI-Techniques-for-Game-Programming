#include "population.h"
#pragma warning( disable:4996)
void GENOME::GeneratePermutation(int NumOfCities )
{
	for(int i=0 ;i<NumOfCities ;i++ )
	{
		vecCities.push_back(i );
	}
	int SwapTime = NumOfCities ;
	while(SwapTime -- )
	{
		int p1 = RandInt(0 ,NumOfCities-1 );
		int p2 = RandInt(0 ,NumOfCities-1 );
		swap(vecCities[p1] ,vecCities[p2] );
	}
	return ;
}

GENOME::GENOME(void ):_Fitness(0 ){;}
GENOME::GENOME(int NumOfCities ):_Fitness(0 )
{
	GeneratePermutation(NumOfCities );
}

double GENOME::Fitness(void )
{
	return _Fitness ;
}

vector<int> GENOME::GetRoute(void )
{
	return vecCities ;
}

#define __RR__GetX(i) (int)(map.vecCities[route[i]]._x)
#define __RR__GetY(i) (int)(map.vecCities[route[i]]._y)

void RouteRenderer(HDC hdc,const MAP &map ,const vector<int> &route )
{
	MoveToEx(hdc ,__RR__GetX(route.size()-1) ,__RR__GetY(route.size()-1) ,NULL );
	for(int i=0 ;i<(int)(route.size()) ;i++ )
	{
		LineTo(hdc ,__RR__GetX(i) ,__RR__GetY(i) );
	}
	return ;
}
#undef __RR__GetX
#undef __RR__GetY


// POPULATION Member Functions 
POPULATION::POPULATION(double RateOfMutation ,double RateOfCrossover ,int SizeOfPopulation ,
					   int NumOfCities ,int WidthOfMap ,int HeightOfMap )
					  :MutationRate(RateOfMutation ),CrossoverRate(RateOfCrossover ),
					   TotalFitness(0 ),ShortestDistance(9999999 ),LongestDistance(0 ),
					   PopulationSize(SizeOfPopulation ),ChromosomeLength(NumOfCities ),
					   FittestGenome(0 ),Generation(0 ),isStarted(false )
{
	pMap = new MAP(WidthOfMap ,HeightOfMap ,NumOfCities );
	GenerateInitialGeneration( );
}

POPULATION::~POPULATION(void )
{
	delete pMap ;
}

void POPULATION::Mutate(vector<int> &genome )
{
	if( RandFloat() < MutationRate )
	{
		int p1 = RandInt(0 ,genome.size()-1 );
		int p2 = RandInt(0 ,genome.size()-1 );
		while(p2 == p1 )p2 = RandInt(0 ,genome.size()-1 );
		swap(genome[p1] ,genome[p2] );
	}
	return ;
}

void POPULATION::Crossover(const vector<int> &parent1 ,const vector<int> &parent2 ,
						   vector<int> &child1 ,vector<int> &child2 )
{
	child1 = parent1 ;
	child2 = parent2 ;
	if( RandFloat() > CrossoverRate || parent1 == parent2 )return ;
	vector<int> map ;
	int i ; // fuck VC++
	for(i=0 ;i<(int)(parent1.size()) ;i++ )map[i] = i ;
	int p1 = RandInt(0 ,parent1.size() );
	int p2 = RandInt(p1+1 ,parent1.size() );
	for(i=p1 ;i<=p2 ;i++ )
	{
		swap(map[parent1[i]] ,map[parent2[i]] ) ;
	}
	for(i=0 ;i<(int)(child1.size()) ;i++ )
	{
		child1[i] = map[child1[i]] ;
		child2[i] = map[child2[i]] ;
	}
	return ;
}

GENOME& POPULATION::RouletteWheelSelection(void )
{
	double NeedFitness = RandFloat() * TotalFitness ;
	double NowFitness = 0 ;
	for(int i=0 ;i<PopulationSize ;i++ )
	{
		NowFitness += vecGenomes[i].Fitness();
		if(NowFitness >= NeedFitness )return vecGenomes[i] ;
	}
	return vecGenomes[PopulationSize-1] ;
}

void POPULATION::CalculatePopulationFitness(void )
{
	int i ;
	for(i=0 ;i<PopulationSize ;i++ )
	{
		vecGenomes[i]._Fitness = pMap->GetTourLength(vecGenomes[i].vecCities );
		if(vecGenomes[i]._Fitness <ShortestDistance )
		{
			ShortestDistance = vecGenomes[i]._Fitness ;
			FittestGenome = i ;
		}
		if(vecGenomes[i]._Fitness >LongestDistance )
		{
			LongestDistance = vecGenomes[i]._Fitness ;
		}
	}
	for(i=0 ;i<PopulationSize ;i++ )
	{
		vecGenomes[i]._Fitness = LongestDistance - vecGenomes[i]._Fitness ;
	}
	return ;
}

void POPULATION::GenerateInitialGeneration(void )
{
	vecGenomes.clear( );
	for(int i=0 ;i<PopulationSize ;i++ )
	{
		vecGenomes.push_back(GENOME(ChromosomeLength ));
	}
	Generation = 0 ;
	ShortestDistance = 9999999 ;
	LongestDistance = 0 ;
	isStarted = false ;
}

void POPULATION::Reset(void )
{
	ShortestDistance = 9999999 ;
	LongestDistance = 0 ;
	TotalFitness = 0 ;
}

void POPULATION::Epoch(void )
{
	Reset() ;
	CalculatePopulationFitness() ;
	if( ShortestDistance <= pMap->BestPossibleLength()+1 )
	{
		isStarted = false ;
		return ;
	}
	vector<GENOME> vecNextGeneration ;
	int i ;
	for(i=0 ;i<__NUM__BEST__KEEP ;i++ )
	{
		vecNextGeneration.push_back(vecGenomes[FittestGenome] );
	}
	while((int)(vecNextGeneration.size()) < PopulationSize )
	{
		GENOME parent1 = RouletteWheelSelection();
		GENOME parent2 = RouletteWheelSelection();

		GENOME child1 ,child2 ;

		Crossover(parent1.vecCities ,parent2.vecCities ,child1.vecCities ,child2.vecCities );

		Mutate(child1.vecCities );
		Mutate(child2.vecCities );

		vecNextGeneration.push_back(child1 );
		vecNextGeneration.push_back(child2 );
	}

	vecGenomes = vecNextGeneration ;
	Generation ++ ;
	return ;
}

void POPULATION::Run(HWND hwnd )
{
	GenerateInitialGeneration();
	isStarted = true ;
	return ;
}

static const char* ReadyToBegin = "Press return to start a new turn";
static const char* Running = "Press space to stop" ;
void POPULATION::Render(HDC hdc ,int cxClient ,int cyClient )
{
	MapRenderer(hdc ,*pMap );
	RouteRenderer(hdc ,*pMap ,vecGenomes[FittestGenome].vecCities );
	const char* out = isStarted ? Running : ReadyToBegin ;
	TextOut(hdc ,cxClient/2 - 3*strlen(out) ,cyClient - 20 ,out ,strlen(out) );
	char generation[30] = "" ;
	sprintf(generation ,"Generation : %d ",Generation );
	TextOut(hdc ,5 ,5 ,generation ,strlen(generation ));
}

void POPULATION::Resize(int cx ,int cy )
{
	pMap->Resize(cx ,cy );
}

void POPULATION::Stop(void )
{
	isStarted = false ;
}

bool POPULATION::Started(void )
{
	return isStarted ;
}