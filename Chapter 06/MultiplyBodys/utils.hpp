#ifndef __UTILS__HPP__INC
#define __UTILS__HPP__INC
#include <cmath>

#include "S2DVector.h"

inline int RandInt(int l ,int r )
{
	return l+(rand()%(r-l+1));
}

inline double RandFloat(void )
{
	return rand()/(RAND_MAX+1.0);
}

inline double RandFloat(double l ,double r )
{
	return l+(rand()/(RAND_MAX+1.0)*(r-l));
}

inline double CalculateDistance(const S2DVector& p1 ,const S2DVector& p2 )
{
	S2DVector dif = p1-p2 ;
	return dif.Length();
}
#endif