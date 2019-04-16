#include "Genome.h"
#pragma warning( disable:4996)
GENOME::GENOME(void ):Fitness(0 )
{
	;
}

GENOME::GENOME(int nc ):Fitness(0 )
{
	vecRoute = GrabPermutation(nc );
}

vector<int> GENOME::GrabPermutation(int &limit )
{
	vector<int> vecPrem ;
	// Generate a vector of 0 - (limit-1) in a random order
	int i ;// fuck VC++
	for(i=0 ;i<limit ;i++ )
	{
		vecPrem.push_back(i );
	}
	for(i=0 ;i<limit ;i++ )
	{
		swap(vecPrem[RandInt(0 ,limit-1 )] ,vecPrem[RandInt(0 ,limit-1 )] );
	}
	return vecPrem ;
}

void POPULATION::MutateEM(vector<int> &chromosome )
{
	if(RandFloat() > MutationRate )return ;
	int pos1 = RandInt(0 ,chromosome.size() ) , pos2 ;
	for(pos2 = pos1 ;pos2 == pos1 ;pos2 = RandInt(0 ,chromosome.size() ) );
	swap(chromosome[pos1] ,chromosome[pos2] );
}

void POPULATION::CrossoverPMX(const vector<int> &pa ,const vector<int> &ma ,
							  vector<int> &s1 ,vector<int> &s2 )
{
	s1 = pa ;
	s2 = ma ;
	if(RandFloat() > CrossoverRate )return ;
	int begin = RandInt(0 ,pa.size()-2 );
	int end = RandInt(begin+1 ,pa.size()-1 );
	for(int i=begin ;i<end+1 ;i++ )
	{
		int gene1 = pa[i] ;
		int gene2 = ma[i] ;
		// Generate Map
		if( gene1 != gene2 )
		{
			int posG1 = *find(s1.begin() ,s1.end() ,gene1 );
			int posG2 = *find(s1.begin() ,s1.end() ,gene2 );
			swap(posG1 ,posG2 );
			;
			posG1 = *find(s2.begin() ,s2.end() ,gene1 );
			posG2 = *find(s2.begin() ,s2.end() ,gene2 );
			swap(posG1 ,posG2 );
		}
	}
	return ;
}

GENOME& POPULATION::RouletteWheelSelection(void )
{
	double Slice = RandFloat() * TotalFitness ;
	double counter = 0 ;
	for(int i=0 ;i<PopulationSize ;i++ )
	{
		counter += vecGenomes[i].Fitness ;
		if( counter >= Slice )return vecGenomes[i] ;
	}
	return vecGenomes[PopulationSize-1] ;
}

void POPULATION::CalculatePopulationFitness(void )
{
	int i ;
	for(i=0 ;i<PopulationSize ;i++ )
	{
		vecGenomes[i].Fitness = pMap->GetTourLength(vecGenomes[i].vecRoute );
		if( vecGenomes[i].Fitness < ShortestDistance )
		{
			ShortestDistance = vecGenomes[i].Fitness ;
			FittestGenome = i ;
		}
		if( vecGenomes[i].Fitness > LongestDistance )
		{
			LongestDistance = vecGenomes[i].Fitness ;
		}
	}
	// Recalculate
	for(i=0 ;i<PopulationSize ;i++ )
	{
		vecGenomes[i].Fitness = LongestDistance - vecGenomes[i].Fitness ;
	}
}

void POPULATION::Reset(void )
{
	ShortestDistance = 999999 ;
	LongestDistance = 0 ;
	TotalFitness = 0 ;
}

void POPULATION::CreateInitialGeneration(void )
{
	vecGenomes.clear( );
	for(int i=0 ;i<PopulationSize ;i++ )
	{
		vecGenomes.push_back(GENOME(ChromosomeLength ));
	}

	Generation = 0 ;
	Reset( );
	isStarted = false ;
}

POPULATION::POPULATION(double RateOfMutation ,double RateOfCrossover ,int SizeOfPopulation ,
					   int NumOfCities ,int WidthOfMap ,int HeightOfMap )
					   :MutationRate(RateOfMutation ),CrossoverRate(RateOfCrossover ),
					   TotalFitness(0 ),ShortestDistance(999999 ),LongestDistance(0 ),
					   PopulationSize(SizeOfPopulation ),ChromosomeLength(NumOfCities ),
					   FittestGenome(0 ),Generation(0 ),isStarted(false )
{
	pMap = new MAP(WidthOfMap ,HeightOfMap ,NumOfCities );
	CreateInitialGeneration( );
}

POPULATION::~POPULATION(void )
{
	delete pMap ;
}

void POPULATION::Epoch(void )
{
	Reset( );
	CalculatePopulationFitness( );
	if( (pMap->GetBestPossibleRoute()) >= ShortestDistance )
	{
		isStarted = false ;
		return ;
	}
	vector<GENOME> vecNewPopulation ;
	for(int i=0 ;i<__BEST__INHERIT__NUM ;i++ )
	{
		vecNewPopulation.push_back(vecGenomes[FittestGenome] );
	}
	while((int)(vecNewPopulation.size()) < PopulationSize )
	{
		GENOME parent1 = RouletteWheelSelection( );
		GENOME parent2 = RouletteWheelSelection( );

		GENOME baby1 ;
		GENOME baby2 ;

		CrossoverPMX(parent1.vecRoute ,parent2.vecRoute ,
					 baby1.vecRoute ,baby2.vecRoute );

		MutateEM(baby1.vecRoute );
		MutateEM(baby2.vecRoute );

		vecNewPopulation.push_back(baby1 );
		vecNewPopulation.push_back(baby2 );
	}

	vecGenomes = vecNewPopulation ;
	Generation++ ;
}

void POPULATION::Run(HWND hwnd )
{
	CreateInitialGeneration( );
	isStarted = true ;
}

#define __PR__INT__GETX(i) (int)(pMap->CitiesCoord[route[i]].x)
#define __PR__INT__GETY(i) (int)(pMap->CitiesCoord[route[i]].y)
void POPULATION::Render(HDC hdc ,int cx ,int cy )
{
	int cs = __CITY__SIZE ;
	for(int i=0 ;i<pMap->NumOfCities ;i++ )
	{
		int x = (int)(pMap->CitiesCoord[i].x) ;
		int y = (int)(pMap->CitiesCoord[i].y) ;
		Ellipse(hdc ,x-cs ,y-cs ,x+cs ,y+cs );
	}
	vector<int> route = vecGenomes[FittestGenome].vecRoute ;
	if( Generation )
	{
		MoveToEx(hdc ,__PR__INT__GETX(0 ),__PR__INT__GETY(0 ),NULL );
		for(int i=1 ;i<(int)(route.size()) ;i++ )
		{
			LineTo(hdc ,__PR__INT__GETX(i ),__PR__INT__GETY(i ));
			char* num = itos(i );
			TextOut(hdc ,__PR__INT__GETX(i ),__PR__INT__GETY(i ),num ,strlen(num ));
			free(num );
		}
		LineTo(hdc ,__PR__INT__GETX(0 ),__PR__INT__GETY(0 ));
	}
	char* sGene = (char*)calloc(25 ,sizeof(char ));
	if (sGene)
	{
		sprintf(sGene, "Generation : %d", Generation);
		TextOut(hdc, 5, 5, sGene, strlen(sGene));
		free(sGene);
	}
}
#undef __PR__INT__GETX
#undef __PR__INT__GETY

void POPULATION::Resize(int cx ,int cy )
{
	;
}

void POPULATION::Stop(void )
{
	;
}

bool POPULATION::Started(void )
{
	return isStarted ;
}