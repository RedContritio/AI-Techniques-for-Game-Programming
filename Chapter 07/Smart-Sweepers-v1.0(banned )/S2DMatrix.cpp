#include "S2DMatrix.h"
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
	C2DMATRIX TransformMatrix ;
	MakeMoveMatrix2D(TransformMatrix ,dx ,dy );
	(*this) *= TransformMatrix ;
}

void C2DMATRIX::Scale(double sx ,double sy )
{
	C2DMATRIX TransformMatrix ;
	MakeScaleMatrix2D(TransformMatrix ,sx ,sy );
	(*this) *= TransformMatrix ;
}

void C2DMATRIX::Rotate(double angle )
{
	C2DMATRIX TransformMatrix ;
	MakeRotateMatrix2D(TransformMatrix ,angle );
	(*this) *= TransformMatrix ;
}

C2DMATRIX& MakeMoveMatrix2D(C2DMATRIX& A ,double dx ,double dy )
{
	A.Identity();
	A.matrix.s[2][0] = dx ;
	A.matrix.s[2][1] = dy ;
	return A ;
}
C2DMATRIX& MakeScaleMatrix2D(C2DMATRIX& A ,double sx ,double sy )
{
	memset(&A.matrix ,0 ,sizeof(A.matrix ));
	A.matrix.s[0][0] = sx ;
	A.matrix.s[1][1] = sy ;
	A.matrix.s[2][2] = 1 ;
	return A ;
}

C2DMATRIX& MakeRotateMatrix2D(C2DMATRIX& A ,double angle )
{
	A.Identity();
	double c = cos(angle ),s = sin(angle );
	A.matrix.s[0][0] = c ;
	A.matrix.s[0][1] = s ;
	A.matrix.s[1][0] = -s ;
	A.matrix.s[1][1] = c ;
	return A ;
}