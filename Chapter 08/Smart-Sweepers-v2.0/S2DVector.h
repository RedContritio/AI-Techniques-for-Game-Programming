#ifndef __S2DVECTOR__DEF
#define __S2DVECTOR__DEF
#include <cmath>

struct S2DVector ;

struct SPoint
{
	double x ,y ;
	SPoint(double ,double );
	SPoint(void );
};

double S2DVLength(S2DVector );
int S2DVSign(const S2DVector& ,const S2DVector& );

struct S2DVector
{
	double x ,y ;
	S2DVector(void );
	S2DVector(double ,double );
	S2DVector& operator += (const S2DVector & );
	S2DVector& operator -= (const S2DVector & );
	S2DVector& operator *= (const double & );
	S2DVector& operator /= (const double & );
	double operator ^= (const S2DVector & );
	friend S2DVector operator + (const S2DVector & ,const S2DVector & );
	friend S2DVector operator - (const S2DVector & ,const S2DVector & );
	friend S2DVector operator * (const S2DVector & ,const double & );
	friend S2DVector operator * (const double & ,const S2DVector & );
	friend S2DVector operator / (const S2DVector & ,const double & );
	friend double operator * (const S2DVector & ,const S2DVector & );
	friend double operator ^ (const S2DVector & ,const S2DVector & );
	operator SPoint(void );
	double Length(void )const ;
	void Normalize(void );
	double Angle(void )const ;
	friend double S2DVLength(S2DVector );
	friend int S2DVSign(const S2DVector& ,const S2DVector& );
};

S2DVector operator + (const S2DVector & ,const S2DVector & );
S2DVector operator - (const S2DVector & ,const S2DVector & );
S2DVector operator * (const S2DVector & ,const double & );
S2DVector operator * (const double & ,const S2DVector & );
S2DVector operator / (const S2DVector & ,const double & );
double operator * (const S2DVector & ,const S2DVector & );
double operator ^ (const S2DVector & ,const S2DVector & );

#endif