#ifndef __UTILS__INC
#define __UTILS__INC

#include <cstdio>
#include <cstdlib>

inline int RandInt(int l ,int r )
{
	return l + (rand() % (r-l+1));
}

inline double RandFloat(void )
{
	return rand() / (RAND_MAX + 1.0 );
}

inline bool RandBool(void )
{
	return rand() % 2 ;
}

inline double RandClamped(void )
{
	return RandFloat() - RandFloat() ;
}

inline int itos(char* str ,int num )
{
	return sprintf_s(str ,15 ,"%d" ,num );
}

inline int ftos(char* str ,double num )
{
	return sprintf_s(str ,20 ,"%g" ,num );
}


inline double Clamp(double &arg ,double l ,double r )
{
	if(arg > r )arg = r ;
	if(arg < l )arg = l ;
	return arg ;
}

#endif