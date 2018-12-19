#ifndef __UTILS__INC
#define __UTILS__INC
#include <cstdlib>
#include <cmath>
#include <cstring>

// Random Function
int RandInt(int ,int );

inline int RandInt(int x ,int y )
{
	return (rand()%(y-x+1))+x;
}

double RandFloat(void );

inline double RandFloat(void )
{
	return (1.0*rand()/RAND_MAX );
}
#endif