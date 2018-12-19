#ifndef __UTILS__INC
#define __UTILS__INC
#include <cstdlib>
inline int RandInt(int l ,int r )
{
	return l + rand() % (r-l+1);
}
#define EPSILON 0.01
inline bool FloatEqual(double a ,double b )
{
	return (a<b+EPSILON ) || (a>b-EPSILON );
}

inline double RandFloat(void )
{
	return rand() / ( RAND_MAX + 1.0 );
}
#endif