#pragma once
#ifndef WIREMATRIX34_H
#define WIREMATRIX34_H

// Matrix34 is a 4x4 matrix with an implicit 4th row of (0, 0, 0, 1).
// It is a row-major matrix, thus it is indexed as Matrix[row][column]

#include "WireSystem.h"
#include "WireTypes.h"
#include "WireVector3.h"

namespace Wire
{

template <class Real>
class Matrix34
{
public:
	// If zero is true, create the zero matrix.  Otherwise, create the
	// identity matrix.
	Matrix34(bool zero = true);

	// input mRC is in row R, column C
	Matrix34(Real m00, Real m01, Real m02, Real m03,
		Real m10, Real m11, Real m12, Real m13,
		Real m20, Real m21, Real m22, Real m23);

	Matrix34& MakeZero();
	Matrix34& MakeIdentity();

	Matrix34& FromAxisAngle(const Vector3<Real>& rAxis, Real angle);

	// get a pointer to the 2-dimensional matrix array
	inline Real (* (Get)())[4] { return mEntry; }

	// arithmetic operations
	inline Matrix34 operator* (Matrix34& rMatrix);

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

#endif /* WIREMATRIX34_H */
