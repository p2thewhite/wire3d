#pragma once
#ifndef WIREMATRIX34_H
#define WIREMATRIX34_H

// Matrix34 is a 4x4 matrix with an implicit 4th row of (0, 0, 0, 1).
// It is a row-major matrix, thus it is indexed as Matrix[row][column]

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
class Matrix34
{
public:
	typedef Real Real4[4];

	// If zero is true, create the zero matrix.  Otherwise, create the
	// identity matrix.
	Matrix34(Bool zero = true);

	// input mRC is in row R, column C
	Matrix34(Real m00, Real m01, Real m02, Real m03,
		Real m10, Real m11, Real m12, Real m13,
		Real m20, Real m21, Real m22, Real m23);

	// Create rotation matrices (positive angle - counterclockwise).  The
	// angle must be in radians, not degrees.
	Matrix34(const Vector3<Real>& rAxis, Real angle);

	Matrix34& MakeZero();
	Matrix34& MakeIdentity();

	Matrix34& FromAxisAngle(const Vector3<Real>& rAxis, Real angle);

	// member operators
	inline operator Real4* ();
//	inline operator const Real4* () const;	// TODO: fix for VC
    inline Real operator() (UInt row, UInt col) const;
    inline Real& operator() (UInt row, UInt col);
	void SetColumn(Int col, const Vector3<Real>& rVector);
	Vector3<Real> GetColumn(Int col) const;

	// arithmetic operations
	inline Matrix34 operator* (const Matrix34& rMatrix) const;
	inline Matrix34 operator* (Real scalar) const;

	// matrix times vector
	inline Vector3<Real> operator* (const Vector3<Real>& rVector) const;

	Matrix34 TimesDiagonal(const Vector3<Real>& rDiag) const;
    Matrix34 Inverse() const;

    // special matrices
    /*WIRE_FOUNDATION_ITEM*/ static const Matrix34 ZERO;
    /*WIRE_FOUNDATION_ITEM*/ static const Matrix34 IDENTITY;

private:
	Real mEntry[3][4];
};

// v^T * M
template <class Real>
inline Vector3<Real> operator* (const Vector3<Real>& rV,
    const Matrix34<Real>& rM);

#ifdef WIRE_WII
#include "Wii/WireMatrix34Wii.inl"
#else
#include "WireMatrix34.inl"
#endif

typedef Matrix34<Float> Matrix34F;

}

#endif
