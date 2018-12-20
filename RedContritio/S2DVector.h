#ifndef __S2DVECTOR__DEF
#define __S2DVECTOR__DEF
#include <cmath>

struct SPoint
{
	double x ,y ;
	SPoint(double ,double );
};

struct S2DVector
{
	double x ,y ;
	S2DVector(void );
	S2DVector(double ,double );
	S2DVector& operator += (const S2DVector & );
	S2DVector& operator -= (const S2DVector & );
	S2DVector& operator *= (const double & );
	S2DVector& operator /= (const double & );
	double Length(void )const ;
	void Normalize(void );
};

S2DVector operator + (const S2DVector & ,const S2DVector & );
S2DVector operator - (const S2DVector & ,const S2DVector & );
S2DVector operator * (const S2DVector & ,const double & );
S2DVector operator * (const double & ,const S2DVector & );
S2DVector operator / (const S2DVector & ,const double & );
#endif