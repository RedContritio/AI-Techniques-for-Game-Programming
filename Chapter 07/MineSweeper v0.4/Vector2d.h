#ifndef VECTOR2D_INC
#define VECTOR2D_INC
#pragma once


namespace RedContritio
{

struct Matrix2d;

struct Vector2d
{
	double x, y;

	Vector2d(double _x = 0.0, double _y = 0.0);
	Vector2d(const Vector2d& pro);
	Vector2d(const Matrix2d &mat);
	~Vector2d(void);

	double length(void) const;
	void Normalize(void);
	Vector2d &operator+=(const Vector2d &rhs);
	Vector2d &operator-=(const Vector2d &rhs);

	Vector2d &operator*=(double rhs);
	Vector2d &operator/=(double rhs);

	Vector2d &operator*=(const Matrix2d &rhs);

	Vector2d &operator-(void);
	Vector2d &operator=(const Vector2d &vec) = default;
	Vector2d &operator=(const Matrix2d &mat);

	double operator*(const Vector2d &rhs) const;
	double operator^(const Vector2d &rhs) const;

	inline double dot(const Vector2d &rhs) const { return this->operator*(rhs); };
	inline double cross(const Vector2d &rhs) const { return this->operator^(rhs); };
};

Vector2d operator+(const Vector2d &lhs, const Vector2d &rhs);
Vector2d operator-(const Vector2d &lhs, const Vector2d &rhs);
Vector2d operator*(double lhs, const Vector2d &rhs);
Vector2d operator*(const Vector2d &lhs, double rhs);
Vector2d operator*(const Vector2d &lhs, const Matrix2d &rhs);

double Vector2dLength(const Vector2d &vec);
Vector2d Normalize(const Vector2d &vec);
int sign(const Vector2d& lhs, const Vector2d& rhs);

}

RedContritio::Vector2d GetRandomVector2d(void);

#endif