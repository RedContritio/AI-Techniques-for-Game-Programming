#ifndef __MAP__DEF
#define __MAP__DEF
#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
using std::vector ;

const int CitiesSize  = 2 ;

struct __RC__COORD
{
	float _x;
	float _y;
	__RC__COORD(void );
	__RC__COORD(float ,float );
};

class MAP
{
	private :
		vector<__RC__COORD> vecCities ;
		int CitiesNum ;
		int MapWidth ;
		int MapHeight ;
		double _BestPossibleLength ;

		void CreateCitiesCircular(void );

		double CalculateDistance(const __RC__COORD & ,const __RC__COORD & );
		void CalculateBestPossibleRoute(void );
	public :
		MAP(int ,int ,int );
		void Resize(const int ,const int );
		double GetTourLength(const vector<int> & );
		double BestPossibleLength(void );

		friend void MapRenderer(HDC ,const MAP & );
		friend void RouteRenderer(HDC ,const MAP & ,const vector<int> & );
};


void MapRenderer(HDC ,const MAP & );
#endif