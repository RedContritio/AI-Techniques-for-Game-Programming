#include "utils.h"
int RandInt(int l ,int r )
{
	return l + rand() % (r -l +1 );
}

double RandFloat(void )
{
	return rand() / (RAND_MAX +1.0 );
}