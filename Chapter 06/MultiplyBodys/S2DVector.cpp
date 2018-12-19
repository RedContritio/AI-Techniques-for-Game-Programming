#include "S2DVector.h"
SPoint::SPoint(double _x ,double _y ):x(_x ),y(_y )
{
	;
}

S2DVector::S2DVector(void ):x(0 ),y(0 )
{
	;
}

S2DVector::S2DVector(double _x = 0 ,double _y = 0 ):x(_x ),y(_y )
{
	;
}

S2DVector& S2DVector::operator += (const S2DVector &A )
{
	this->x += A.x ;
	this->y += A.y ;
	return (*this );
}

S2DVector& S2DVector::operator -= (const S2DVector &A )
{
	this->x -= A.x ;
	this->y -= A.y ;
	return (*this );
}

S2DVector& S2DVector::operator *= (const double &rhs )
{
	this->x *= rhs ;
	this->y *= rhs ;
	return (*this );
}

S2DVector& S2DVector::operator /= (const double &rhs )
{
	this->x /= rhs ;
	this->y /= rhs ;
	return (*this );
}

double S2DVector::Length(void )const
{
	return sqrt(x*x + y*y );
}

void S2DVector::Normalize(void )
{
	double len = Length() ;
	if(len )
	{
		x /= len ;
		y /= len ;
	}
	return ;
}

S2DVector operator + (const S2DVector &lhs ,const S2DVector &rhs )
{
	S2DVector result(lhs );
	result += rhs ;
	return result ;
}

S2DVector operator - (const S2DVector &lhs ,const S2DVector &rhs )
{
	S2DVector result(lhs );
	result -= rhs ;
	return result ;
}

S2DVector operator * (const S2DVector &lhs ,const double &rhs )
{
	S2DVector result(lhs );
	result *= rhs ;
	return result ;
}

S2DVector operator * (const double &lhs ,const S2DVector &rhs )
{
	S2DVector result(rhs );
	result *= lhs ;
	return result ;
}

S2DVector operator / (const S2DVector &lhs ,const double &rhs )
{
	S2DVector result(lhs );
	result /= rhs ;
	return result ;
}