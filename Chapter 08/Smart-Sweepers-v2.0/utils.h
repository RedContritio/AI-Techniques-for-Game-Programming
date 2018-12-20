#ifndef UTILS__INC
#define UTILS__INC
#pragma once
#include <cstdlib>
#include <cassert>

#define RandInt(l,r) FastRandInt(l,r)
inline int FastRandInt(int l ,int r ){return l+rand()%(r-l);}
inline int RealRandInt(int l ,int r ){return l+(int)((r-l)*(rand()/(RAND_MAX+1.0 )));}

inline double RandClamped(void ){return 2.0*rand()/RAND_MAX-1.0 ;}
inline double RandFloat(void ){return rand()/(RAND_MAX+1.0);}

#define Clamp(x,l,len) ClampByLength(x,l,len)

void ClampByLength(int& x ,int l ,int len );
void ClampByLength(double& x ,double l ,double len );

#endif