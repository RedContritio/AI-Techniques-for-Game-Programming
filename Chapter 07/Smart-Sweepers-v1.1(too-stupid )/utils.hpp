#ifndef UTILS__INC
#define UTILS__INC
#pragma once
#include "S2DVector.h"
#include <cstdlib>

inline double RandClamped(void ){return ((2.0*rand())/RAND_MAX)-1.0 ;}

inline double RandFloat(void ){return 1.0*rand()/RAND_MAX ;}

inline int RandInt(int l ,int r ){return (rand()%(r-l+1))+l ;}

inline double Clamp(double& arg ,double l ,double r )
{
	if(arg <l )arg = l ;
	if(arg >r )arg = r ;
	return arg ;
}

inline void RectLimit(S2DVector& v ,int left ,int top ,int width ,int height )
{
	while(v.x <left )v.x +=width ;
	while(v.x >left +width )v.x -=width ;
	while(v.y <top )v.y += height ;
	while(v.y >top +height )v.y -= height ;
	return ;
}
#endif