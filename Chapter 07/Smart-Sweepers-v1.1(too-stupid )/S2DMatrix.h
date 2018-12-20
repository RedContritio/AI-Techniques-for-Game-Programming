#ifndef __C2DMATRIX__DEF
#define __C2DMATRIX__DEF
#include "S2DVector.h"
#include <cstring>
#include <cmath>

#include <vector>
using std::vector ;

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
	void SeriousTrans(double dx ,double dy ,double sx ,double sy ,double angle );
	void SetMoveTrans(double dx ,double dy );
	void SetScaleTrans(double sx ,double sy );
	void SetRotateTrans(double angle );
	void SetSeriousTrans(double dx ,double dy ,double sx ,double sy ,double angle );
	static void TransformSPoints(vector<SPoint>& points ,double dx ,double dy ,double sx ,double sy ,double angle );
	inline double& x(void )
	{
		return matrix.l[0];
	}
	inline double& y(void )
	{
		return matrix.l[1];
	}
};
#endif