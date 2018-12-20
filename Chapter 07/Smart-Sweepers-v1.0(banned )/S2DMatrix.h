#ifndef __C2DMATRIX__DEF
#define __C2DMATRIX__DEF
#include <cstring>
#include <cmath>

struct C2DMATRIX
{
	union S2DMATRIX
	{
		double l[9] ;
		double s[3][3] ;
		struct 
		{
			double _00 ,_01 ,_02 ;
			double _10 ,_11 ,_12 ;
			double _20 ,_21 ,_22 ;
		}se ;
	}matrix;

	C2DMATRIX(void );
	C2DMATRIX(double ,double );
	~C2DMATRIX(void );
	C2DMATRIX operator += (const C2DMATRIX & );
	C2DMATRIX operator -= (const C2DMATRIX & );
	C2DMATRIX operator *= (const C2DMATRIX & );
	C2DMATRIX operator /= (const C2DMATRIX & );
	C2DMATRIX operator + (const C2DMATRIX & ) const ;
	C2DMATRIX operator - (const C2DMATRIX & ) const ;
	C2DMATRIX operator * (const C2DMATRIX & ) const ;
	C2DMATRIX operator / (const C2DMATRIX & ) const ;
	C2DMATRIX operator / (const double & ) const ;
	C2DMATRIX Adjoint(void )const ;
	C2DMATRIX Inverse(void )const ;
	void Identity(void );
	void Move(double dx ,double dy );
	void Scale(double sx ,double sy );
	void Rotate(double angle );
	inline double x(void )
	{
		return matrix.l[0];
	}
	inline double y(void )
	{
		return matrix.l[1];
	}
};

C2DMATRIX& MakeMoveMatrix2D(C2DMATRIX& A ,double dx ,double dy );
C2DMATRIX& MakeScaleMatrix2D(C2DMATRIX& A ,double sx ,double sy );
C2DMATRIX& MakeRotateMatrix2D(C2DMATRIX& ,double angle );
#endif