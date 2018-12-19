#include "utils.h"
#pragma warning( disable:4996)
// inline functions 

//--------------------------------------//
//--------------------------------------//

char* itos(int num )
{
	char* str = (char*)malloc(0x10*sizeof(char) );
	memset(str ,0 ,0x10*sizeof(char) );
	sprintf(str ,"%d" ,num );
	return str ;
}

char* ftos(double num )
{
	char* str = (char*)malloc(0x20*sizeof(char) );
	memset(str ,0 ,0x20*sizeof(char) );
	sprintf(str ,"%g" ,num );
	return str ;
}

//--------------------------------------//
//--------------------------------------//

template<typename T>
void Clamp(T &arg ,T min ,T max )
{
	if(arg < min )
	{
		arg = min ;
	}
	if(arg > max )
	{
		arg = max ;
	}
}