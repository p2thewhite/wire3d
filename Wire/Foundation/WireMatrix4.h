#pragma once
#ifndef WIREMATRIX4_H
#define WIREMATRIX4_H

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
//	inline operator const Real4* () const; // TODO: fix for VC

private:
	Real mEntry[4][4];
};

#include "WireMatrix4.inl"

typedef Matrix4<Float> Matrix4F;

}

#endif
