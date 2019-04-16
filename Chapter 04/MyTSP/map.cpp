#include "map.h"
#include "LogSpawner.h"
using RedContritio::LogSpawner ;
extern LogSpawner loger ;
__RC__COORD::__RC__COORD(void ){;}
__RC__COORD::__RC__COORD(float x ,float y ):_x(x ),_y(y ){;}

const static double pi = 1.0*acos(-1 );
static inline double square(double x ){return x*x ;}
void MAP::CreateCitiesCircular(void )
{
	const int margin = 30 ;
	const int radius = (int)(min(MapWidth ,MapHeight )/2 - margin) ;
	__RC__COORD center(0.5f*MapWidth ,0.5f*MapHeight );
	const double SegmentArc = 2.0 * pi / CitiesNum ;
	double angle = 0 ;

	vector<__RC__COORD> vecTempCity ;
	for(int i=0 ;i<CitiesNum ;i++ )
	{
		__RC__COORD city ;
		city._x = (float)(center._x + radius * cos(angle ));
		city._y = (float)(center._y + radius * sin(angle ));
		vecTempCity.push_back(city );
		
		angle += SegmentArc ;
	}
	vecCities = vecTempCity ;
}


double MAP::CalculateDistance(const __RC__COORD &A ,const __RC__COORD &B )
{
	return sqrt(square(A._x -B._x )+square(A._y -B._y ));
}

void MAP::CalculateBestPossibleRoute(void )
{
	double totalDistance = 0 ;
	for(int i=0 ;i<CitiesNum-1 ;i++ )
	{
		totalDistance += CalculateDistance(vecCities[i] ,vecCities[i+1] );
	}
	totalDistance += CalculateDistance(vecCities[CitiesNum-1] ,vecCities[0] );
	_BestPossibleLength = totalDistance ;
}

MAP::MAP(int width ,int height ,int NumOfCities )
		:MapWidth(width ),MapHeight(height ),CitiesNum(NumOfCities )
{
	CreateCitiesCircular( );
	CalculateBestPossibleRoute( );
}

void MAP::Resize(const int width ,const int height )
{
	MapWidth = width ;
	MapHeight = height ;

	vecCities.clear( );
	CreateCitiesCircular( );
	CalculateBestPossibleRoute( );
}

double MAP::GetTourLength(const vector<int> &route )
{
	double totalDistance = 0 ;
	for(int i=0 ;i<CitiesNum-1 ;i++ )
	{
		totalDistance += CalculateDistance(vecCities[route[i]] ,vecCities[route[i+1]] );
	}
	totalDistance += CalculateDistance(vecCities[route[CitiesNum-1]] ,vecCities[route[0]] );
	return totalDistance ;
}

double MAP::BestPossibleLength(void )
{
	return _BestPossibleLength ;
}

void MapRenderer(HDC hdc ,const MAP &map )
{
	for(int i=0 ;i<map.CitiesNum ;i++ )
	{
		int x = (int)(map.vecCities[i]._x);
		int y = (int)(map.vecCities[i]._y);
		Ellipse(hdc ,x-CitiesSize ,y-CitiesSize ,x+CitiesSize ,y+CitiesSize );
	}
}