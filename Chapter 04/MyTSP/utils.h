#ifndef __UTILS__INC
#define __UTILS__INC
#include <cstdlib>
inline int RandInt(int l ,int r )
{
	// assert(r >= l && r-l < 100 )
	return l+(rand()%(r-l+1));
}
inline double RandFloat(void )
{
	return 1.0 * rand() / (1.0 + RAND_MAX);
}
#endif