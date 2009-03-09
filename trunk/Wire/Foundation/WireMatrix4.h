#pragma once
#ifndef WIREMATRIX4_H
#define WIREMATRIX4_H

#include "WireSystem.h"
#include "WireTypes.h"

namespace Wire
{

template <class Real>
class Matrix4
{
public:
	typedef Real Real4[4];

	// If zero is true, create the zero matrix.  Otherwise, create the
	// identity matrix.
	Matrix4(Bool zero = true);

	// input mRC is in row R, column C
	Matrix4(Real m00, Real m01, Real m02, Real m03,
		Real m10, Real m11, Real m12, Real m13,
		Real m20, Real m21, Real m22, Real m23,
		Real m30, Real m31, Real m32, Real m33);

	Matrix4& MakeZero();
	Matrix4& MakeIdentity();

	// access operators
	inline operator Real4* ();
	inline operator const Real4* () const;

private:
	Real mEntry[4][4];
};

#include "WireMatrix4.inl"

typedef Matrix4<Float> Matrix4f;

}

#endif /* WIREMATRIX4_H */
