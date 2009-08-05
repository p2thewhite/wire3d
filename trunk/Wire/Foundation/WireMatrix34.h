#pragma once
#ifndef WIREMATRIX34_H
#define WIREMATRIX34_H

// Matrix34 is a 4x4 matrix with an implicit 4th row of (0, 0, 0, 1).
// It is a row-major matrix, thus it is indexed as Matrix[row][column]

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
	Matrix34(const Vector3<Real>& rkAxis, Real fAngle);

	Matrix34& MakeZero();
	Matrix34& MakeIdentity();

	Matrix34& FromAxisAngle(const Vector3<Real>& rAxis, Real angle);

	// member operators
	inline operator Real4* ();
//	inline operator const Real4* () const;	// TODO: fix for VC
	void SetColumn(Int col, const Vector3<Real>& rVector);
	Vector3<Real> GetColumn(Int col) const;

	// arithmetic operations
	inline Matrix34 operator* (const Matrix34& rMatrix) const;
	inline Matrix34 operator* (Real scalar) const;

	// matrix times vector
	inline Vector3<Real> operator* (const Vector3<Real>& rVector) const;

	Matrix34 TimesDiagonal(const Vector3<Real>& rDiag) const;

private:
	Real mEntry[3][4];
};

#ifdef WIRE_WII
#include "Wii/WireMatrix34Wii.inl"
#else
#include "WireMatrix34.inl"
#endif

typedef Matrix34<Float> Matrix34f;

}

#endif
