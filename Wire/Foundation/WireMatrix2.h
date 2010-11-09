#pragma once
#ifndef WIREMATRIX2_H
#define WIREMATRIX2_H

// Matrix operations are applied on the left. For example, given a matrix M
// and a vector V, matrix-times-vector is M*V. That is, V is treated as a
// column vector. Some graphics APIs use V*M where V is treated as a row
// vector. In this context the "M" matrix is really a transpose of the M as
// represented in Wire. Similarly, to apply two matrix operations M0 and M1,
// in that order, you compute M1*M0 so that the transform of a vector is
// (M1*M0)*V = M1*(M0*V). Some graphics APIs use M0*M1, but again these
// matrices are the transpose of those as represented in Wire. You must
// therefore be careful about how you interface the transformation code with
// graphics APIS.

// Rotation matrices are of the form
//   R = cos(t) -sin(t)
//       sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

#include "WireTypes.h"
#include "WireVector2.h"

namespace Wire
{

template <class Real>
class Matrix2
{
public:
	// If zero is true, create the zero matrix. Otherwise, create the
	// identity matrix.
	Matrix2(Bool zero = true);

	// input mRC is in row R, column C.
	Matrix2(Real m00, Real m01, Real m10, Real m11);

	// Create rotation matrix (positive angle - counterclockwise).  The
	// angle must be in radians, not degrees.
	Matrix2(Real angle);

	// Create matrices based on row or column vector input
	Matrix2(const Vector2<Real>& rU, const Vector2<Real>& rV, Bool isColumn);

	// create various matrices
	void MakeZero ();
	void MakeIdentity ();
	void FromAngle(Real angle);

	// member access
	inline operator const Real* () const;
	inline operator Real* ();
	inline Real operator() (UInt row, UInt col) const;
	inline Real& operator() (UInt row, UInt col);
	void SetColumn(UInt col, const Vector2<Real>& rV);
	Vector2<Real> GetColumn (UInt col) const;

	// arithmetic operations
	inline Matrix2 operator* (const Matrix2& rM) const;
	inline Matrix2 operator* (Real scalar) const;

	// matrix times vector
	inline Vector2<Real> operator* (const Vector2<Real>& rV) const; // M * v

	// other operations
	Matrix2 Inverse() const;

	// special matrices
	/*WIRE_FOUNDATION_ITEM*/ static const Matrix2 ZERO;
	/*WIRE_FOUNDATION_ITEM*/ static const Matrix2 IDENTITY;

private:
	// matrix stored in row-major order
	Real mEntry[4];
};

// v^T * M
template <class Real>
inline Vector2<Real> operator* (const Vector2<Real>& rV,
	const Matrix2<Real>& rM);

#include "WireMatrix2.inl"

typedef Matrix2<Float> Matrix2F;
typedef Matrix2<Double> Matrix2D;

}

#endif
