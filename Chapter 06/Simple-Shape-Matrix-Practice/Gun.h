#ifndef __GUN__DEF
#define __GUN__DEF
#include "C2DMATRIX.h"
#include <windows.h>
#include <cmath>
#include <vector>
using std::vector ;

#include "defines.h"

struct SPoint
{
	double x , y ;
	SPoint(double ,double );
};

class GUN
{
	public :
		SPoint dPos ;
		double Rotation ;
		double Scale ;
		vector<SPoint> vecGunVB ;
		vector<SPoint> vecGunVBTrans ;
		GUN(double ,double ,double ,double );
		void WorldTransform(void );
		void Render(HDC );
		void Update(void );
};
#endif