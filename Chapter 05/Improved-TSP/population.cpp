#include "population.h"
#pragma warning( disable:4996)
static int AM_Generation = 0 ;
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
	Mutate = MutateEM ;
	Crossover = CrossoverPMX ;
	SelectParent = RouletteWheelSelection ;
	GenerateInitialGeneration( );
}

POPULATION::~POPULATION(void )
{
	delete pMap ;
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
		GENOME parent1 = SelectParent(vecGenomes ,TotalFitness );
		GENOME parent2 = SelectParent(vecGenomes ,TotalFitness );

		GENOME child1 ,child2 ;
		child1.vecCities = parent1.vecCities ;
		child2.vecCities = parent2.vecCities ;

		
		if( RandFloat() < CrossoverRate && parent1.vecCities != parent2.vecCities )Crossover(parent1.vecCities ,parent2.vecCities ,child1.vecCities ,child2.vecCities );

		if( RandFloat() < MutationRate )Mutate(child1.vecCities );
		if( RandFloat() < MutationRate )Mutate(child2.vecCities );

		vecNextGeneration.push_back(child1 );
		vecNextGeneration.push_back(child2 );
	}

	vecGenomes = vecNextGeneration ;
	Generation ++ ;
	AM_Generation = Generation ;
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

void POPULATION::SelectMutationType(int type )
{
	switch(type )
	{
		case MutationType_Exchange :
		{
			Mutate = MutateEM ;
			break ;
		}
		case MutationType_Displacement :
		{
			Mutate = MutateDM ;
			break ;
		}
		case MutationType_Scramble :
		{
			Mutate = MutateSM ;
			break ;
		}
		case MutationType_Insert :
		{
			Mutate = MutateSM ;
			break ;
		}
		case MutationType_Automatic :
		default :
		{
			Mutate = MutateAM ;
			break ;
		}
	}
	return ;
}

void POPULATION::SelectCrossoverType(int type )
{
	switch(type )
	{
		case CrossoverType_PartiallyMapping :
		{
			Crossover = CrossoverPMX ;
			break ;
		}
		case CrossoverType_OrderBasedOperator :
		{
			Crossover = CrossoverOBX ;
			break ;
		}
		case CrossoverType_PositionBasedOperator :
		{
			Crossover = CrossoverPBX ;
			break ;
		}
		default :
		{
			MessageBoxEx(NULL ,"Test Crossover Type switch" ,"NOTICE" ,MB_OK ,0 );
			Crossover = CrossoverPMX ;
			break ;
		}
	}
	return ;
}

void MutateEM(vector<int> &genome )
{
	int p1 = RandInt(0 ,genome.size()-1 );
	int p2 = RandInt(0 ,genome.size()-1 );
	while(p2 == p1 )p2 = RandInt(0 ,genome.size()-1 );
	swap(genome[p1] ,genome[p2] );
	return ;
}

void MutateDM(vector<int> &genome )
{
	const int MinSpanLength = 3 ;
	int beg ,end ;
	SectionChoose(beg ,end ,genome.size()-1 ,MinSpanLength );
	std::vector<int>::iterator iBegin = genome.begin() + beg ;
	std::vector<int>::iterator iEnd = genome.begin() + end ;

	vector<int> Section ;
	Section.assign(iBegin ,iEnd );
	genome.erase(iBegin ,iEnd );

	std::vector<int>::iterator iInsert = genome.begin() + RandInt(0 ,genome.size()-1 );
	genome.insert(iInsert ,Section.begin() ,Section.end() );
	return ;
}

void MutateSM(vector<int> &genome )
{
	const int MinSpanLength = 2 ;
	int beg = RandInt(0 ,genome.size()-MinSpanLength );
	int end = RandInt(beg+MinSpanLength-1 ,genome.size()-1 );
	for(int i=0 ;i<end-beg+1 ;i++ )
	{
		int p1 = RandInt(beg ,end );
		int p2 = RandInt(beg ,end );
		swap(genome[p1] ,genome[p2] );
	}
	return ;
}

void MutateIM(vector<int> &genome )
{
	std::vector<int>::iterator iPos ;
	iPos = genome.begin() + RandInt(0 ,genome.size()-1 );
	int tmpCity = *iPos ;
	genome.erase(iPos );
	iPos = genome.begin() + RandInt(0 ,genome.size()-1 );
	genome.insert(iPos ,tmpCity );
	return ;
}

void MutateAM(vector<int> & genome)
{
	void (*const MutationFunction[4])(vector<int> & ) = {MutateEM ,MutateDM ,MutateIM ,MutateSM };
	int tgm = AM_Generation % 40 ;
	int tgp = tgm / 10 ;
	MutationFunction[tgp ](genome );
}

void CrossoverOBX(const vector<int> &parent1 ,const vector<int> &parent2 ,
						   vector<int> &child1 ,vector<int> &child2 )
{
	vector<int> Positions ;
	vector<int> tmpCities ;
	{
		int pos = RandInt(0 ,parent1.size()-2 );
		while(pos < (int)(parent1.size()) )
		{
			Positions.push_back(pos );
			pos = RandInt(pos+1 ,parent1.size() );
		}
		for(int i=0 ;i<(int)(Positions.size()) ;i++ )
		{
			tmpCities[i] = parent1[Positions[i]] ;
		}
	}
	int i = 0 , j = 0 ; // fuck VC++
	int k = 0 ;
	for(i=0 ;i<(int)(parent2.size()) ;i++ )
	{
		std::vector<int>::iterator it = std::find(tmpCities.begin() ,tmpCities.end() ,parent2[k] );
		if( it != tmpCities.end() )
		{
			child2[i] = tmpCities[k++];
		}
	}

	tmpCities.clear() ;
	k = 0 ;
	{
		for(int i=0 ;i<(int)(Positions.size()) ;i++ )
		{
			tmpCities[i] = parent2[Positions[i]] ;
		}
	}

	for(i=0 ;i<(int)(parent2.size()) ;i++ )
	{
		std::vector<int>::iterator it = std::find(tmpCities.begin() ,tmpCities.end() ,parent1[k] );
		if( it != tmpCities.end() )
		{
			child1[i] = tmpCities[k++];
		}
	}
	return ;
}

void CrossoverPBX(const vector<int> &parent1 ,const vector<int> &parent2 ,
						   vector<int> &child1 ,vector<int> &child2 )
{
	vector<int> Positions ;
	vector<int> tmpCities ;
	{
		int pos = RandInt(0 ,parent1.size()-2 );
		while(pos < (int)(parent1.size()) )
		{
			Positions.push_back(pos );
			pos = RandInt(pos+1 ,parent1.size() );
		}
	}
	int i = 0 , j = 0 ; // fuck VC++
	int k = 0 ;
	{
		child1.clear( );
		for(i=0 ;i<(int)(parent2.size()) ;i++ )
		{
			std::vector<int>::iterator it = std::find(Positions.begin() ,Positions.end() ,i );
			if( it == Positions.end( )) // not exist
			{
				child1[i] = parent2[k] ;
				k ++ ;
			}
			else
			{
				child1[i] = parent1[i] ;
				j ++ ;
			}
			
		}
	}

	k = 0 ;
	{
		child2.clear( );
		for(i=0 ;i<(int)(parent1.size()) ;i++ )
		{
			std::vector<int>::iterator it = std::find(Positions.begin() ,Positions.end() ,i );
			if( it == Positions.end() ) // not exist
			{
				child2[i] = parent1[k] ;
				k ++ ;
			}
			else
			{
				child2[i] = parent2[i] ;
				j ++ ;
			}
			
		}
	}
	return ;
}

void CrossoverPMX(const vector<int> &parent1 ,const vector<int> &parent2 ,
						   vector<int> &child1 ,vector<int> &child2 )
{
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


void SectionChoose(int &Begin ,int &End ,int MaxLength,int MinLength )
{
	Begin = RandInt(0 ,MaxLength - MinLength );
	End = RandInt(Begin+MinLength-1 ,MaxLength );
	return ;
}

GENOME& RouletteWheelSelection(vector<GENOME> &vecGenomes ,double TotalFitness )
{
	double NeedFitness = RandFloat() * TotalFitness ;
	double NowFitness = 0 ;
	for(int i=0 ;i<(int)(vecGenomes.size()) ;i++ )
	{
		NowFitness += vecGenomes[i].Fitness();
		if(NowFitness >= NeedFitness )return vecGenomes[i] ;
	}
	return vecGenomes[vecGenomes.size()-1] ;
}