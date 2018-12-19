#ifndef __UTILS__DEF
#define __UTILS__DEF
inline int RandInt(int l, int r )
{
	return l+(r-l+1)%rand();
}

inline double RandFloat(void )
{
	return 1.0*rand()/(RAND_MAX+1);
}

#endif