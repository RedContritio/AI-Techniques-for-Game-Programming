#include "Map.h"
static double square(double x );
R_COORD::R_COORD(void )
{
	x = 0 ;
	y = 0 ;
}

R_COORD::R_COORD(float x ,float y ):x(x ),y(y )
{
	;
}

void MAP::CreateCitiesCircular(void )
{
	const int margin = 50 ;
	double Radius ;
	int LengthLimitation = min(MapWidth ,MapHeight );
	Radius = (LengthLimitation /2.0 ) - margin ;
	R_COORD origin((float)(MapWidth /2) , (float)(MapHeight /2));
}

double MAP::CalculateDistance(const R_COORD &A ,const R_COORD &B )
{
	return sqrt( square(A.x - B.x)+square(A.y - B.y) );
}

void MAP::CalculateBestRoute(void )
{
	BestPossibleRoute = 0 ;
	int i = 0 ;
	int N = CitiesCoord.size() ;
	for(i=0 ;i<N-1 ;i++ )
	{
		BestPossibleRoute += CalculateDistance(CitiesCoord[i] ,CitiesCoord[i+1] );
	}
	BestPossibleRoute += CalculateDistance(CitiesCoord[N-1] ,CitiesCoord[0] );
}

MAP::MAP(int Width ,int Height ,int NumofCities )
	:MapWidth(Width ),MapHeight(Height ) ,NumOfCities(NumofCities )
{
	;
}

void MAP::Resize(const int width ,const int height )
{
	MapWidth = width ;
	MapHeight = height ;
	CitiesCoord.clear( );
	CreateCitiesCircular( );
	CalculateBestRoute( );
}

double MAP::GetTourLength(const vector<int> &Route )
{
	int N = Route.size() ;
	double TotalLength = 0 ;
	for(int i=0 ;i<N-1 ;i++ )
	{
		TotalLength += CalculateDistance(CitiesCoord[Route[i]] ,CitiesCoord[Route[i+1]] );
	}
	TotalLength += CalculateDistance(CitiesCoord[Route[0]] ,CitiesCoord[Route[N-1]] );
	return TotalLength ;
}

double MAP::GetBestPossibleRoute(void )const 
{
	return BestPossibleRoute ;
}

static inline double square(double x )
{
	return x*x;
}