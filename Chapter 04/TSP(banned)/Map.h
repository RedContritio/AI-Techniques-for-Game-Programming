#ifndef __MAP__DEF
#define __MAP__DEF

#include <cmath>
#include <windows.h>
#include <vector>
using std::vector ;

const double pi = 3.1415926535897932384626433832795 ;

struct R_COORD
{
	float x ,y ;
	R_COORD(void );
	R_COORD(float ,float );
};

class MAP
{
	private :
		int NumOfCities ;
		int MapWidth ;
		int MapHeight ;
		double BestPossibleRoute ;
		void CreateCitiesCircular(void );
		double CalculateDistance(const R_COORD & ,const R_COORD & );
		void CalculateBestRoute(void );
	public :
		vector<R_COORD> CitiesCoord ;
		MAP(int ,int ,int );
		void Resize(const int ,const int );
		double GetTourLength(const vector<int> & ) ;
		double GetBestPossibleRoute(void )const ;
		friend class POPULATION ;
};
#endif