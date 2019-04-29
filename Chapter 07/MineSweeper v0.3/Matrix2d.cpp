#include "Matrix2d.h"
#include "Vector2d.h"
#include <cmath>

namespace RedContritio
{

Matrix2d::Matrix2d(double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22)
{
	m_data[0][0] = v00; m_data[0][1] = v01; m_data[0][2] = v02;
	m_data[1][0] = v10; m_data[1][1] = v11; m_data[1][2] = v12;
	m_data[2][0] = v20; m_data[2][1] = v21; m_data[2][2] = v22;
}

Matrix2d::Matrix2d(const Vector2d &vec)
{
	m_data[0][0] = vec.x; m_data[0][1] = vec.y; m_data[0][2] = 1;
	m_data[1][0] = 0    ; m_data[1][1] = 0    ; m_data[1][2] = 0;
	m_data[2][0] = 0    ; m_data[2][1] = 0    ; m_data[2][2] = 0;
}

Matrix2d &Matrix2d::operator+=(const Matrix2d &rhs)
{
	m_data[0][0] += rhs.m_data[0][0]; m_data[0][1] += rhs.m_data[0][1]; m_data[0][2] += rhs.m_data[0][2];
	m_data[1][0] += rhs.m_data[1][0]; m_data[1][1] += rhs.m_data[1][1]; m_data[1][2] += rhs.m_data[1][2];
	m_data[2][0] += rhs.m_data[2][0]; m_data[2][1] += rhs.m_data[2][1]; m_data[2][2] += rhs.m_data[2][2];
	return *this;
}

Matrix2d &Matrix2d::operator-=(const Matrix2d &rhs)
{
	m_data[0][0] -= rhs.m_data[0][0]; m_data[0][1] -= rhs.m_data[0][1]; m_data[0][2] -= rhs.m_data[0][2];
	m_data[1][0] -= rhs.m_data[1][0]; m_data[1][1] -= rhs.m_data[1][1]; m_data[1][2] -= rhs.m_data[1][2];
	m_data[2][0] -= rhs.m_data[2][0]; m_data[2][1] -= rhs.m_data[2][1]; m_data[2][2] -= rhs.m_data[2][2];
	return *this;
}

Matrix2d &Matrix2d::operator*=(double rhs)
{
	m_data[0][0] *= rhs; m_data[0][1] *= rhs; m_data[0][2] *= rhs;
	m_data[1][0] *= rhs; m_data[1][1] *= rhs; m_data[1][2] *= rhs;
	m_data[2][0] *= rhs; m_data[2][1] *= rhs; m_data[2][2] *= rhs;
	return *this;
}

Matrix2d &Matrix2d::operator*=(const Matrix2d &rhs)
{
	double v00 = m_data[0][0]* rhs.m_data[0][0] + m_data[0][1]* rhs.m_data[1][0] + m_data[0][2]* rhs.m_data[2][0];
	double v01 = m_data[0][0]* rhs.m_data[0][1] + m_data[0][1]* rhs.m_data[1][1] + m_data[0][2]* rhs.m_data[2][1];
	double v02 = m_data[0][0]* rhs.m_data[0][2] + m_data[0][1]* rhs.m_data[1][2] + m_data[0][2]* rhs.m_data[2][2];
	double v10 = m_data[1][0]* rhs.m_data[0][0] + m_data[1][1]* rhs.m_data[1][0] + m_data[1][2]* rhs.m_data[2][0];
	double v11 = m_data[1][0]* rhs.m_data[0][1] + m_data[1][1]* rhs.m_data[1][1] + m_data[1][2]* rhs.m_data[2][1];
	double v12 = m_data[1][0]* rhs.m_data[0][2] + m_data[1][1]* rhs.m_data[1][2] + m_data[1][2]* rhs.m_data[2][2];
	double v20 = m_data[2][0]* rhs.m_data[0][0] + m_data[2][1]* rhs.m_data[1][0] + m_data[2][2]* rhs.m_data[2][0];
	double v21 = m_data[2][0]* rhs.m_data[0][1] + m_data[2][1]* rhs.m_data[1][1] + m_data[2][2]* rhs.m_data[2][1];
	double v22 = m_data[2][0]* rhs.m_data[0][2] + m_data[2][1]* rhs.m_data[1][2] + m_data[2][2]* rhs.m_data[2][2];

	m_data[0][0] = v00; m_data[0][1] = v01; m_data[0][2] = v02;
	m_data[1][0] = v10; m_data[1][1] = v11; m_data[1][2] = v12;
	m_data[2][0] = v20; m_data[2][1] = v21; m_data[2][2] = v22;

	return *this;
}

Matrix2d &Matrix2d::operator/=(double rhs)
{
	m_data[0][0] /= rhs; m_data[0][1] /= rhs; m_data[0][2] /= rhs;
	m_data[1][0] /= rhs; m_data[1][1] /= rhs; m_data[1][2] /= rhs;
	m_data[2][0] /= rhs; m_data[2][1] /= rhs; m_data[2][2] /= rhs;
	return *this;
}

Matrix2d &Matrix2d::operator=(const Vector2d &vec)
{
	m_data[0][0] = vec.x; m_data[0][1] = vec.y; m_data[0][2] = 1;
	m_data[1][0] = 0; m_data[1][1] = 0; m_data[1][2] = 0;
	m_data[2][0] = 0; m_data[2][1] = 0; m_data[2][2] = 0;
	return *this;
}

Matrix2d &Matrix2d::unitize(void)
{
	m_data[0][0] = 1; m_data[0][1] = 0; m_data[0][2] = 0;
	m_data[1][0] = 0; m_data[1][1] = 1; m_data[1][2] = 0;
	m_data[2][0] = 0; m_data[2][1] = 0; m_data[2][2] = 1;
	return *this;
	return *this;
}

Matrix2d Matrix2d::eye(void)
{
	return Matrix2d(1, 0, 0,
					0, 1, 0,
					0, 0, 1);
}

Matrix2d Matrix2d::zero(void)
{
	return Matrix2d(0, 0, 0,
					0, 0, 0,
					0, 0, 0);
}

#define MD m_data
Matrix2d Matrix2d::adjugate(void) const
{
	return Matrix2d(MD[1][1]*MD[2][2] - MD[1][2]*MD[2][1], MD[0][2]*MD[2][1] - MD[0][1]*MD[2][2], MD[0][1]*MD[1][2] - MD[0][2]*MD[1][1],
					MD[1][2]*MD[2][0] - MD[1][0]*MD[2][2], MD[0][0]*MD[2][2] - MD[0][2]*MD[2][0], MD[0][2]*MD[1][0] - MD[0][0]*MD[1][2],
					MD[1][0]*MD[2][1] - MD[1][1]*MD[2][0], MD[0][1]*MD[2][0] - MD[0][0]*MD[2][1], MD[0][0]*MD[1][1] - MD[0][1]*MD[1][0]
					);
}

Matrix2d Matrix2d::inverse(void) const
{
	return adjugate()/determinant(*this);
}
#undef MD

Matrix2d Matrix2d::move(double dx, double dy)
{
	return Matrix2d( 1,  0,  0,
					 0,  1,  0,
					dx, dy,  1);
}

Matrix2d Matrix2d::move(const Vector2d &dv)
{
	return Matrix2d(	1,		0,		0,
						0,		1,		0,
					 dv.x,	 dv.y,		1);
}

Matrix2d Matrix2d::scale(double sx, double sy)
{
	return Matrix2d(sx,  0,  0,
					 0, sy,  0,
					 0,  0,  1);
}

Matrix2d Matrix2d::scale(double sv)
{
	return Matrix2d(sv,  0,  0,
					 0, sv,  0,
					 0,  0,  1);
}

Matrix2d Matrix2d::rotate(double theta)
{
	double c = cos(theta), s = sin(theta);
	return Matrix2d( c,  s,  0,
					-s,  c,  0,
					 0,  0,  1);
}

Matrix2d Matrix2d::transform(double dx, double dy, double sx, double sy, double theta)
{
	double c = cos(theta), s = sin(theta);
	return Matrix2d( c*sx, s*sx,	0,
					-s*sy, c*sy,	0,
					   dx,   dy,	1);
}


double determinant(const Matrix2d &mat)
{
	return mat.m_data[0][0] *mat.m_data[1][1] *mat.m_data[2][2]
		 + mat.m_data[0][1] *mat.m_data[1][2] *mat.m_data[2][0] 
		 + mat.m_data[0][2] *mat.m_data[1][0] *mat.m_data[2][1]
		 - mat.m_data[0][0] *mat.m_data[1][2] *mat.m_data[2][1]
		 - mat.m_data[0][1] *mat.m_data[1][0] *mat.m_data[2][2]
		 - mat.m_data[0][2] *mat.m_data[1][1] *mat.m_data[2][0];
}

Matrix2d operator+(const Matrix2d &lhs, const Matrix2d &rhs)
{
	return Matrix2d(lhs) += rhs;
}

Matrix2d operator-(const Matrix2d &lhs, const Matrix2d &rhs)
{
	return Matrix2d(lhs) -= rhs;
}

Matrix2d operator*(const Matrix2d &lhs, double rhs)
{
	return Matrix2d(lhs) *= rhs;
}

Matrix2d operator*(double lhs, const Matrix2d &rhs)
{
	return Matrix2d(rhs) *= lhs;
}

Matrix2d operator*(const Matrix2d &lhs, const Matrix2d &rhs)
{
	return Matrix2d(lhs) *= rhs;
}

Matrix2d operator/(const Matrix2d &lhs, double rhs)
{
	return Matrix2d(lhs) /= rhs;
}

}