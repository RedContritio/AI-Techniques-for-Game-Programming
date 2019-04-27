#ifndef UTILS_INC
#define UTILS_INC
#pragma once

#include <cstdlib>
#include <cmath>

namespace RedContritio
{
namespace Utils
{

inline int RandInt(int l, int r) { return l+rand()%(r-l); } // [l, r)

inline float RandFloat(float l, float r) { return (float)(l+(rand()/(RAND_MAX+1.0))*((double)r-l)); } // [l, r)

inline double RandFloat(double l, double r) { return l+(rand()/(RAND_MAX+1.0))*(r-l); } // [l, r)

inline double RandFloat(void) { return (rand()/(RAND_MAX+1.0)); } // [0, 1)

inline bool RandBool(void) { return (rand()&1) ? true : false; }

inline double RandClamped(void) { return RandFloat(-1.0, 1.0); } // (-1, 1)

inline double Sigmoid(double value, double bound) { return (1.0 / ( 1.0 + exp( -value / bound))); }

inline double Clamp(double value, double left, double right)
{
	value = value<left ? left : value;
	value = value>right ? right : value;
	return value;
}

}
}
#endif