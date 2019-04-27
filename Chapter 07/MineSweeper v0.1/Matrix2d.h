#ifndef MATRIX2D_INC
#define MATRIX2D_INC
#pragma once

#include <iostream>

namespace RedContritio
{
struct Vector2d;

struct Matrix2d
{
	double m_data[3][3];
	Matrix2d(double v00 = 0, double v01 = 0, double v02 = 0, double v10 = 0, double v11 = 0, double v12 = 0, double v20 = 0, double v21 = 0, double v22 = 0);
	Matrix2d(const Vector2d &vec);
	Matrix2d(const Matrix2d &obj) = default;
	~Matrix2d(void) = default;

	Matrix2d &operator+=(const Matrix2d &rhs);
	Matrix2d &operator-=(const Matrix2d &rhs);
	
	Matrix2d &operator*=(double rhs);
	Matrix2d &operator/=(double rhs);

	Matrix2d &operator*=(const Matrix2d &rhs);

	Matrix2d &operator=(const Matrix2d &mat) = default;
	Matrix2d &operator=(const Vector2d &vec);

	Matrix2d &unitize(void); // Make this matrix a eye matrix

	static Matrix2d eye(void);
	static Matrix2d zero(void);

	Matrix2d adjugate(void) const;
	Matrix2d inverse(void) const;

	static Matrix2d I(void) { return eye(); }
	static Matrix2d Identity(void) { return eye(); }

	static Matrix2d move(double dx, double dy);
	static Matrix2d move(const Vector2d &dv);
	static Matrix2d scale(double sx, double sy);
	static Matrix2d scale(double sv);
	static Matrix2d rotate(double theta);
	static Matrix2d transform(double dx = 0, double dy = 0, double sx = 1, double sy = 1, double theta = 0);

};

double determinant(const Matrix2d &mat);
Matrix2d operator+(const Matrix2d &lhs, const Matrix2d &rhs);
Matrix2d operator-(const Matrix2d &lhs, const Matrix2d &rhs);
Matrix2d operator*(const Matrix2d &lhs, double rhs);
Matrix2d operator*(double lhs, const Matrix2d &rhs);
Matrix2d operator*(const Matrix2d &lhs, const Matrix2d &rhs);
Matrix2d operator/(const Matrix2d &lhs, double rhs);


}
#endif