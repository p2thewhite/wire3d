#pragma once
#ifndef WIREMATRIX3_H
#define WIREMATRIX3_H

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

#include "WireTypes.h"
#include "WireVector3.h"

namespace Wire
{

template <class Real>
class Matrix3
{
public:
	// If zero is true, create the zero matrix. Otherwise, create the
	// identity matrix.
	Matrix3(Bool zero = true);

	// input mRC is in row R, column C.
	Matrix3(Real m00, Real m01, Real m02,
		Real m10, Real m11, Real m12,
		Real m20, Real m21, Real m22);

	// Create rotation matrices (positive angle - counterclockwise).  The
	// angle must be in radians, not degrees.
	Matrix3(const Vector3<Real>& rAxis, Real angle);

	// create various matrices
	Matrix3& MakeZero();
	Matrix3& MakeIdentity();
    Matrix3& FromAxisAngle(const Vector3<Real>& rAxis, Real angle);

	// member access
	inline operator const Real* () const;
	inline operator Real* ();
	inline Real operator() (UInt row, UInt col) const;
	inline Real& operator() (UInt row, UInt col);
	void SetColumn(UInt col, const Vector3<Real>& rVector);
	Vector3<Real> GetColumn(UInt col) const;

	// arithmetic operations
	inline Matrix3 operator* (const Matrix3& rMatrix) const;
	inline Matrix3 operator* (Real scalar) const;

	// matrix times vector
	inline Vector3<Real> operator* (const Vector3<Real>& rVector) const;

	// other operations
	Matrix3 TimesDiagonal(const Vector3<Real>& rDiag) const;
	Matrix3 Inverse() const;

	// special matrices
	/*WIRE_FOUNDATION_ITEM*/ static const Matrix3 ZERO;
	/*WIRE_FOUNDATION_ITEM*/ static const Matrix3 IDENTITY;

private:
    Real mEntry[9];
};

// v^T * M
template <class Real>
inline Vector3<Real> operator* (const Vector3<Real>& rV,
	const Matrix3<Real>& rM);

#include "WireMatrix3.inl"

typedef Matrix3<Float> Matrix3F;
typedef Matrix3<Double> Matrix3D;

}

#endif
