#include "utils.h"
#include "Population.h"
#pragma warning( disable:4996)
GENOME::GENOME(void ):Fitness(0 ){;}
GENOME::GENOME(const int NumOfBits ):Fitness(0 )
{
	for(int i=0 ;i<NumOfBits ;i++ )
	{
		vecBits.push_back(RandInt(0 ,1 ));
	}
}

void POPULATION::Mutate(vector<int> &vecBits )
{
	for(int i=0 ;i<vecBits.size() ;i++ )
	{
		if(RandFloat() < MutationRate )
		{
			vecBits[i] = !vecBits[i] ;
		}
	}
}

void POPULATION::Crossover(const vector<int> &mum ,const vector<int> &dad ,
			   vector<int> &baby1 ,vector<int> &baby2 )
{
	if( RandFloat() > CrossoverRate || mum == dad )
	{
		baby1 = mum ;
		baby2 = dad ;
		return ;
	}

	int ExchangePos = RandInt(0 ,ChromosomeLength-1 );
	int i ;
	for(i=0 ;i<ExchangePos ;i++ )
	{
		baby1.push_back(mum[i] );
		baby2.push_back(dad[i] );
	}
	;
	for( ;i<mum.size() ;i++ )
	{
		baby1.push_back(dad[i] );
		baby2.push_back(mum[i] );
	}
	return ;
}

GENOME& POPULATION::RouletteWheelSelection(void )
{
	double fSlice = RandFloat() * TotalFitnessScore ;
	double cfTotal = 0 ;
	int SelectedGenome = 0 ;

	for(int i=0 ;i<PopSize ;i++ )
	{
		cfTotal += vecGenome[i].Fitness ;
		if( cfTotal > fSlice )
		{
			SelectedGenome = i ;
			break ;
		}
	}
	return vecGenome[SelectedGenome] ;
}

void POPULATION::UpdateFitnessScore(void  )
{
	FittestGenome = 0 ;
	BestFitnessScore = 0 ;
	TotalFitnessScore = 0 ;

	MAP tempMemory ;
	for(int i=0 ;i<PopSize ;i++ )
	{
		vector<int> vecDirections = Decode(vecGenome[i].vecBits );
		vecGenome[i].Fitness = Map.TestRoute(vecDirections ,tempMemory );

		TotalFitnessScore += vecGenome[i].Fitness ;
		
		if(vecGenome[i].Fitness > BestFitnessScore )
		{
			BestFitnessScore = vecGenome[i].Fitness ;
			FittestGenome = i ;
			Brain = tempMemory ;
			if(1 == vecGenome[i].Fitness )
			{
				isBusy = false ;
			}
		}

		tempMemory.ResetMemory();
	}

	vector<int> vecDirections = Decode(vecGenome[FittestGenome].vecBits );
	printf("Generation : %d \n" ,Generation );
	for(int j=0 ;j<vecDirections.size() ;j++ )
	{
		printf("%d ",vecDirections[j] );
	}
	printf("\n\n");
}

vector<int> POPULATION::Decode(const vector<int> &Bits )
{
	vector<int> directions ;
	for(int gene=0 ;gene <Bits.size() ;gene += GeneLength )
	{
		vector<int> curGene ;
		for(int bit=0 ;bit <GeneLength ;bit++ )
		{
			curGene.push_back(Bits[gene+bit] );
		}
		directions.push_back(BinToInt(curGene ));
	}
	return directions ;
}

int POPULATION::BinToInt(const vector<int> &Gene )
{
	int val = 0 ;
	int multiplier = 1 ;
	for(int i=Gene.size()-1 ;i+1 ;i-- )
	{
		val += Gene[i] * multiplier ;
		multiplier *= 2 ;
	}
	return val ;
}

void POPULATION::CreateInitialPopulation(void )
{
	vecGenome.clear( );
	for(int i=0 ;i<PopSize ;i++ )
	{
		vecGenome.push_back(GENOME(ChromosomeLength ));
	}
	
	Generation = 0 ;
	FittestGenome = 0 ;
	BestFitnessScore = 0 ;
	TotalFitnessScore = 0 ;
}

POPULATION::POPULATION(double CrossRate ,double MutaRate ,int PopSize ,int NumBits ,
					   int GeneLen ):CrossoverRate(CrossRate ),MutationRate(MutaRate ),
					   PopSize(PopSize ),ChromosomeLength(NumBits ),TotalFitnessScore(0 ),
					   Generation(0 ),GeneLength(GeneLen ),isBusy(false )
{
	CreateInitialPopulation( );
}

void POPULATION::Run(HWND hwnd )
{
	CreateInitialPopulation( );
	isBusy = true ;
}

void POPULATION::Render(int cx ,int cy ,HDC surface )
{
	Map.Render(cx ,cy ,surface );

	Brain.MemoryRender(cx ,cy ,surface );

	char str[256] = "Generation : ";
	sprintf(str+strlen(str ) ,"%d ",Generation );
	TextOut(surface ,5 ,0 ,str ,strlen(str ));

	if(!isBusy )
	{
		char RestartStr[256] = "Press Enter to start a new turn .";
		int RStrlen = strlen(RestartStr );
		TextOut(surface ,cx/2 - 3*RStrlen ,cy-20 ,RestartStr ,RStrlen );
	}
	else
	{
		char StartStr[256] = "Space to stop .";
		int SStrlen = strlen(StartStr );
		TextOut(surface ,cx/2 - 3*SStrlen ,cy-20 ,StartStr ,SStrlen );
	}
	return ;
}

void POPULATION::Epoch(void )
{
	UpdateFitnessScore( );
	int NewBabies = 0 ;
	vector<GENOME> BabyGenomes ;
	while(NewBabies < PopSize )
	{
		GENOME mum = RouletteWheelSelection( );
		GENOME dad = RouletteWheelSelection( );

		GENOME baby1 ,baby2 ;
		Crossover(mum.vecBits ,dad.vecBits ,
				  baby1.vecBits ,baby2.vecBits );

		Mutate(baby1.vecBits );
		Mutate(baby2.vecBits );

		BabyGenomes.push_back(baby1 );
		BabyGenomes.push_back(baby2 );
		NewBabies += 2 ;
	}
	vecGenome = BabyGenomes ;
	Generation ++ ;
}

int POPULATION::GetGeneration(void )
{
	return Generation ;
}

int POPULATION::GetFittest(void )
{
	return FittestGenome ;
}

bool POPULATION::Started(void )
{
	return isBusy ;
}

void POPULATION::Stop(void )
{
	isBusy = false ;
}