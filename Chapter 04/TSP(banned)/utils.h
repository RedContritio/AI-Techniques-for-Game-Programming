#ifndef __UTILS__INC
#define __UTILS__INC
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "defines.h"
int RandInt(int ,int );
double RandFloat(void );
bool RandBool(void );
double RandClamped(void ); // What does "clamped" mean ?

inline int RandInt(int left ,int right )
{
	return left + (rand() % (right-left+1) );
}

inline double RandFloat(void )
{
	return rand()/(RAND_MAX+1.0);
}

inline bool RandBool(void )
{
	return (rand()&1);
}

inline double RandClamped(void )
{
	return RandFloat() - RandFloat() ;
}
//--------------------------------------//
//--------------------------------------//
char* itos(int );
char* ftos(double );

//--------------------------------------//
//--------------------------------------//
template<typename T>
void Clamp(T & ,T ,T );
#endif