#include "utils.h"

void ClampByLength(int& x ,int l ,int len )
{
	assert(len >0 );
	while(x <l )x+=len ;
	while(x >l+len )x-=len ;
}

void ClampByLength(double& x ,double l ,double len )
{
	assert(len >0 );
	while(x <l )x+=len ;
	while(x >l+len )x-=len ;
}