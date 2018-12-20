#include "S2DMatrix.h"
#include "S2DVector.h"

#include <vector>
using std::vector ;

const double DC_EPSILON = 0.001 ;
static double DeterminantCalculate(const double det[3][3] )
{
	double result = 0 ;
	result += det[0][0] * det[1][1] * det[2][2] ;
	result += det[0][1] * det[1][2] * det[2][0] ;
	result += det[0][2] * det[1][0] * det[2][1] ;
	result -= det[0][0] * det[1][2] * det[2][1] ;
	result -= det[0][1] * det[1][0] * det[2][2] ;
	result -= det[0][2] * det[1][1] * det[2][0] ;
	return result ;
}

C2DMATRIX::C2DMATRIX(void )
{
	Identity() ;
}

C2DMATRIX::C2DMATRIX(double x ,double y )
{
	memset(&matrix ,0 ,sizeof(matrix ));
	matrix.s[0][0] = x ;
	matrix.s[0][1] = y ;
	matrix.s[0][2] = 1 ;
}

C2DMATRIX::~C2DMATRIX(void )
{
	;
}

C2DMATRIX C2DMATRIX::operator += (const C2DMATRIX &A )
{
	C2DMATRIX result ;
	result = (*this ) + A ;
	matrix.l[0] = result.matrix.l[0] ;
	matrix.l[1] = result.matrix.l[1] ;
	matrix.l[2] = result.matrix.l[2] ;
	matrix.l[3] = result.matrix.l[3] ;
	matrix.l[4] = result.matrix.l[4] ;
	matrix.l[5] = result.matrix.l[5] ;
	matrix.l[6] = result.matrix.l[6] ;
	matrix.l[7] = result.matrix.l[7] ;
	matrix.l[8] = result.matrix.l[8] ;
	return (*this );
}

C2DMATRIX C2DMATRIX::operator -= (const C2DMATRIX &A )
{
	C2DMATRIX result ;
	result = (*this ) - A ;
	matrix.l[0] = result.matrix.l[0] ;
	matrix.l[1] = result.matrix.l[1] ;
	matrix.l[2] = result.matrix.l[2] ;
	matrix.l[3] = result.matrix.l[3] ;
	matrix.l[4] = result.matrix.l[4] ;
	matrix.l[5] = result.matrix.l[5] ;
	matrix.l[6] = result.matrix.l[6] ;
	matrix.l[7] = result.matrix.l[7] ;
	matrix.l[8] = result.matrix.l[8] ;
	return (*this );
}

C2DMATRIX C2DMATRIX::operator *= (const C2DMATRIX &A )
{
	C2DMATRIX result ;
	result = (*this ) * A ;
	matrix.l[0] = result.matrix.l[0] ;
	matrix.l[1] = result.matrix.l[1] ;
	matrix.l[2] = result.matrix.l[2] ;
	matrix.l[3] = result.matrix.l[3] ;
	matrix.l[4] = result.matrix.l[4] ;
	matrix.l[5] = result.matrix.l[5] ;
	matrix.l[6] = result.matrix.l[6] ;
	matrix.l[7] = result.matrix.l[7] ;
	matrix.l[8] = result.matrix.l[8] ;
	return (*this );
}

C2DMATRIX C2DMATRIX::operator /= (const C2DMATRIX &A )
{
	C2DMATRIX result ;
	result = (*this ) / A ;
	matrix.l[0] = result.matrix.l[0] ;
	matrix.l[1] = result.matrix.l[1] ;
	matrix.l[2] = result.matrix.l[2] ;
	matrix.l[3] = result.matrix.l[3] ;
	matrix.l[4] = result.matrix.l[4] ;
	matrix.l[5] = result.matrix.l[5] ;
	matrix.l[6] = result.matrix.l[6] ;
	matrix.l[7] = result.matrix.l[7] ;
	matrix.l[8] = result.matrix.l[8] ;
	return (*this );
}

C2DMATRIX C2DMATRIX::operator + (const C2DMATRIX &A )const
{
	C2DMATRIX result ;
	result.matrix.se._00 = this->matrix.se._00 + A.matrix.se._00 ;
	result.matrix.se._01 = this->matrix.se._01 + A.matrix.se._01 ;
	result.matrix.se._02 = this->matrix.se._02 + A.matrix.se._02 ;
	result.matrix.se._10 = this->matrix.se._10 + A.matrix.se._10 ;
	result.matrix.se._11 = this->matrix.se._11 + A.matrix.se._11 ;
	result.matrix.se._12 = this->matrix.se._12 + A.matrix.se._12 ;
	result.matrix.se._20 = this->matrix.se._20 + A.matrix.se._20 ;
	result.matrix.se._21 = this->matrix.se._21 + A.matrix.se._21 ;
	result.matrix.se._22 = this->matrix.se._22 + A.matrix.se._22 ;
	return result ;
}

C2DMATRIX C2DMATRIX::operator - (const C2DMATRIX &A )const
{
	C2DMATRIX result ;
	result.matrix.se._00 = this->matrix.se._00 - A.matrix.se._00 ;
	result.matrix.se._01 = this->matrix.se._01 - A.matrix.se._01 ;
	result.matrix.se._02 = this->matrix.se._02 - A.matrix.se._02 ;
	result.matrix.se._10 = this->matrix.se._10 - A.matrix.se._10 ;
	result.matrix.se._11 = this->matrix.se._11 - A.matrix.se._11 ;
	result.matrix.se._12 = this->matrix.se._12 - A.matrix.se._12 ;
	result.matrix.se._20 = this->matrix.se._20 - A.matrix.se._20 ;
	result.matrix.se._21 = this->matrix.se._21 - A.matrix.se._21 ;
	result.matrix.se._22 = this->matrix.se._22 - A.matrix.se._22 ;
	return result ;
}

C2DMATRIX C2DMATRIX::operator * (const C2DMATRIX &A ) const 
{
	C2DMATRIX result ;
	result.matrix.se._00 =(this->matrix.se._00)*(A.matrix.se._00) +(this->matrix.se._01)*(A.matrix.se._10) +(this->matrix.se._02)*(A.matrix.se._20);
	result.matrix.se._01 =(this->matrix.se._00)*(A.matrix.se._01) +(this->matrix.se._01)*(A.matrix.se._11) +(this->matrix.se._02)*(A.matrix.se._21);
	result.matrix.se._02 =(this->matrix.se._00)*(A.matrix.se._02) +(this->matrix.se._01)*(A.matrix.se._12) +(this->matrix.se._02)*(A.matrix.se._22);
	result.matrix.se._10 =(this->matrix.se._10)*(A.matrix.se._00) +(this->matrix.se._11)*(A.matrix.se._10) +(this->matrix.se._12)*(A.matrix.se._20);
	result.matrix.se._11 =(this->matrix.se._10)*(A.matrix.se._01) +(this->matrix.se._11)*(A.matrix.se._11) +(this->matrix.se._12)*(A.matrix.se._21);
	result.matrix.se._12 =(this->matrix.se._10)*(A.matrix.se._02) +(this->matrix.se._11)*(A.matrix.se._12) +(this->matrix.se._12)*(A.matrix.se._22);
	result.matrix.se._20 =(this->matrix.se._20)*(A.matrix.se._00) +(this->matrix.se._21)*(A.matrix.se._10) +(this->matrix.se._22)*(A.matrix.se._20);
	result.matrix.se._21 =(this->matrix.se._20)*(A.matrix.se._01) +(this->matrix.se._21)*(A.matrix.se._11) +(this->matrix.se._22)*(A.matrix.se._21);
	result.matrix.se._22 =(this->matrix.se._20)*(A.matrix.se._02) +(this->matrix.se._21)*(A.matrix.se._12) +(this->matrix.se._22)*(A.matrix.se._22);
	return result ;
}

C2DMATRIX C2DMATRIX::operator / (const C2DMATRIX &A ) const
{
	C2DMATRIX result ;
	C2DMATRIX tmp = A.Inverse() ;
	result = (*this) * tmp ;
	return result ;
}

C2DMATRIX C2DMATRIX::operator / (const double &div ) const 
{
	C2DMATRIX result = *this;
	for(int i=0 ;i<9 ;i++ )
	{
		result.matrix.l[i] /= div ;
	}
	return result ;
}

C2DMATRIX C2DMATRIX::Adjoint(void )const 
{
	C2DMATRIX result ;
	result.matrix.se._00 = matrix.se._11 * matrix.se._22 - matrix.se._12 * matrix.se._21 ;
	result.matrix.se._01 = matrix.se._12 * matrix.se._20 - matrix.se._10 * matrix.se._22 ;
	result.matrix.se._02 = matrix.se._10 * matrix.se._21 - matrix.se._11 * matrix.se._20 ;
	result.matrix.se._10 = matrix.se._02 * matrix.se._21 - matrix.se._01 * matrix.se._22 ;
	result.matrix.se._11 = matrix.se._00 * matrix.se._22 - matrix.se._02 * matrix.se._20 ;
	result.matrix.se._12 = matrix.se._01 * matrix.se._20 - matrix.se._00 * matrix.se._21 ;
	result.matrix.se._20 = matrix.se._01 * matrix.se._12 - matrix.se._02 * matrix.se._11 ;
	result.matrix.se._21 = matrix.se._02 * matrix.se._20 - matrix.se._00 * matrix.se._22 ;
	result.matrix.se._22 = matrix.se._00 * matrix.se._11 - matrix.se._01 * matrix.se._10 ;
	return result ;
}

C2DMATRIX C2DMATRIX::Inverse(void ) const 
{
	C2DMATRIX result ;
	double det =  DeterminantCalculate(matrix.s ) ;
	if( det >= 0+DC_EPSILON || det <= 0-DC_EPSILON )
	{
		result = this->Adjoint() ;
		result = result / det ;
	}
	return result ;
}

void C2DMATRIX::Identity(void )
{
	memset(&matrix ,0 ,sizeof(matrix ));
	matrix.s[0][0] = 1 ;
	matrix.s[1][1] = 1 ;
	matrix.s[2][2] = 1 ;
}

void C2DMATRIX::Move(double dx ,double dy )
{
	matrix.l[0] += dx ;
	matrix.l[1] += dy ;
	return ;
}

void C2DMATRIX::Scale(double sx ,double sy )
{
	matrix.l[0] *= sx ;
	matrix.l[1] *= sy ;
	return ;
}

void C2DMATRIX::Rotate(double angle )
{
	double v_sin =sin(angle ),v_cos =cos(angle );
	double x =matrix.l[0] ,y =matrix.l[1] ;
	matrix.l[0] = x *v_cos -y *v_sin ;
	matrix.l[1] = x *v_sin +y *v_cos ;
	return ;
}

void C2DMATRIX::SeriousTrans(double dx ,double dy ,double sx, double sy, double angle)
{
	double v_sin =sin(angle ),v_cos =cos(angle );
	double x =matrix.l[0] ,y =matrix.l[1] ;
	matrix.l[0] = x *sx *v_cos - y *sy *v_sin +dx ;
	matrix.l[1] = x *sx *v_sin + y *sy *v_cos +dy ;
	return ;
}

void C2DMATRIX::SetMoveTrans(double dx, double dy)
{
	Identity();
	matrix.s[2][0] = dx ;
	matrix.s[2][1] = dy ;
	return ;
}

void C2DMATRIX::SetScaleTrans(double sx, double sy)
{
	memset(&matrix ,0 ,sizeof(matrix ));
	matrix.s[0][0] = sx ;
	matrix.s[1][1] = sy ;
	matrix.s[2][2] = 1 ;
	return ;
}

void C2DMATRIX::SetRotateTrans(double angle)
{
	memset(&matrix ,0 ,sizeof(matrix ));
	double c = cos(angle ),s = sin(angle );
	matrix.s[0][0] = c ;
	matrix.s[0][1] = s ;
	matrix.s[1][0] = -s ;
	matrix.s[1][1] = c ;
	return ;
}

void C2DMATRIX::SetSeriousTrans(double dx, double dy, double sx, double sy, double angle)
{
	double v_sin =sin(angle );
	double v_cos =cos(angle );
	matrix.l[0] = sx *v_cos ;	matrix.l[1] = sx *v_sin ;	matrix.l[2] = 0 ;
	matrix.l[3] =-sy *v_sin ;	matrix.l[4] = sy *v_cos ;	matrix.l[5] = 0 ;
	matrix.l[6] = dx        ;	matrix.l[7] = dy        ;	matrix.l[8] = 1 ;
	return ;
}

void C2DMATRIX::TransformSPoints(vector<SPoint>& points, double dx, double dy, double sx, double sy, double angle)
{
	double v_sin =sin(angle ),v_cos =cos(angle );
	double a = sx *v_cos ,b =-sy *v_sin ;
	double c = sx *v_sin ,d = sy *v_cos ;
	for(size_t i=0 ;i<points.size() ;i++ )
	{
		double x =points[i].x ,y =points[i].y ;
		points[i].x = a *x +b *y +dx ;
		points[i].y = c *x +d *y +dy ;
	}
	return ;
}