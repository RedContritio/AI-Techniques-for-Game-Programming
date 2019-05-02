#include "Vector2d.h"
#include "Matrix2d.h"
#include "Params.h"
#include "utils.hpp"

#include <cmath>

namespace RedContritio
{


Vector2d::Vector2d(double _x, double _y) : x(_x), y(_y) {}

Vector2d::Vector2d(const Vector2d &pro) : x(pro.x), y(pro.y) {}

Vector2d::Vector2d(const Matrix2d &mat) : x(mat.m_data[0][0]), y(mat.m_data[0][1]) {}

Vector2d::~Vector2d(void) {}

Vector2d &Vector2d::operator+=(const Vector2d &rhs)
{
	x += rhs.x, y += rhs.y;
	return *this;
}

Vector2d &Vector2d::operator-=(const Vector2d &rhs)
{
	x -= rhs.x, y -= rhs.y;
	return *this;
}

Vector2d &Vector2d::operator*=(double rhs)
{
	x *= rhs, y *= rhs;
	return *this;
}
Vector2d &Vector2d::operator/=(double rhs)
{
	x /= rhs, y /= rhs;
	return *this;
}

Vector2d &Vector2d::operator*=(const Matrix2d &rhs)
{
	double nx = x* rhs.m_data[0][0] + y* rhs.m_data[1][0] + rhs.m_data[2][0];
	double ny = x* rhs.m_data[0][1] + y* rhs.m_data[1][1] + rhs.m_data[2][1];
	x = nx, y = ny;
	return *this;
}

Vector2d &Vector2d::operator-(void)
{
	x = -x, y = -y;
	return *this;
}

Vector2d &Vector2d::operator=(const Matrix2d &mat)
{
	x = mat.m_data[0][0], y = mat.m_data[0][1];
	return *this;
}

double Vector2d::operator*(const Vector2d &rhs) const
{
	return x*rhs.x + y*rhs.y;
}
double Vector2d::operator^(const Vector2d &rhs) const
{
	return x*rhs.y - y*rhs.x; // lhs ^ rhs == lhs cross rhs
}
double Vector2d::length(void) const
{
	return sqrt(x*x + y*y);
}

void Vector2d::Normalize(void)
{
	double len = length();
	x /= len, y /= len;
	return;
}

Vector2d operator+(const Vector2d &lhs, const Vector2d &rhs)
{
	return Vector2d(lhs) += rhs;
}
Vector2d operator-(const Vector2d &lhs, const Vector2d &rhs)
{
	return Vector2d(lhs) -= rhs;
}

Vector2d operator*(const Vector2d &lhs, double rhs)
{
	return Vector2d(lhs) *= rhs;
}

Vector2d operator*(const Vector2d &lhs, const Matrix2d &rhs)
{
	return Vector2d(lhs) *=rhs;
}

Vector2d operator*(double lhs, const Vector2d &rhs)
{
	return Vector2d(rhs) *= lhs;
}

double Vector2dLength(const Vector2d &vec)
{
	return vec.length();
}

Vector2d Normalize(const Vector2d &vec)
{
	Vector2d t(vec);
	t.Normalize();
	return t;
}

int sign(const Vector2d &lhs, const Vector2d &rhs) // rhs ÔÚ lhs ÄæÊ±Õë
{
	return (lhs^rhs) > 0 ? 1 : -1;
}

}

using RedContritio::Utils::RandFloat;
RedContritio::Vector2d GetRandomVector2d(void)
{
	return RedContritio::Vector2d(RandFloat()* Params::WindowWidth, RandFloat()* Params::WindowHeight);
}