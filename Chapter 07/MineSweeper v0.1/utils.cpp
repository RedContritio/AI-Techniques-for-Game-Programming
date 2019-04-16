#include "utils.h"
inline int RandInt(int l ,int r )
{
	return l + rand()%(r-l+1);
}

inline double RandFloat(void )
{
	return rand()%(RAND_MAX+1 );
}
