#pragma once
#ifndef WIREVECTOR3_H
#define WIREVECTOR3_H

#include "WireTypes.h"
#include "WireMath.h"

namespace Wire
{

template <class Real>
class Vector3
{
public:
	// construction
	Vector3();  // uninitialized
	Vector3(Real x, Real y, Real z);

	// coordinate access
	inline operator Real* ();
	inline operator const Real* () const;
	inline Real X() const;
	inline Real& X();
	inline Real Y() const;
	inline Real& Y();
	inline Real Z() const;
	inline Real& Z();

	// assignment
	inline Vector3& operator= (const Vector3& rVector);

	// comparison
 	inline Bool operator== (const Vector3& rVector) const;
 	inline Bool operator!= (const Vector3& rVector) const;

	// arithmetic operations
	inline Vector3 operator+ (const Vector3& rVector) const;
	inline Vector3 operator- (const Vector3& rVector) const;
	inline Vector3 operator* (Real scalar) const;
	inline Vector3 operator/ (Real scalar) const;
	inline Vector3 operator- () const;

	// arithmetic updates
	inline Vector3& operator+= (const Vector3& rVector);
	inline Vector3& operator-= (const Vector3& rVector);
	inline Vector3& operator*= (Real scalar);
	inline Vector3& operator/= (Real scalar);

	// vector operations
	inline Real Length() const;
	inline Real SquaredLength() const;
	inline Real Dot(const Vector3& rVector) const;
	inline Real Normalize();

	// The cross products are computed using the right-handed rule. Be aware
	// that some graphics APIs use a left-handed rule.  If you have to compute
	// a cross product with these functions and send the result to the API
	// that expects left-handed, you will need to change sign on the vector
	// (replace each component value c by -c).
	inline Vector3 Cross(const Vector3& rVector) const;

	// Gram-Schmidt orthonormalization. Take linearly independent vectors
	// U, V, and W and compute an orthonormal set (unit length, mutually
	// perpendicular).
	static void Orthonormalize(Vector3& rU, Vector3& rV, Vector3& rW);

	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 ZERO;    // (0,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 UNIT_X;  // (1,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 UNIT_Y;  // (0,1,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 UNIT_Z;  // (0,0,1)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 ONE;     // (1,1,1)

private:
#ifdef WIRE_WII
#include "Wii/WireOgcDefines.h"
	Vector mTuple;
#else
	Real mTuple[3];
#endif
};

// arithmetic operations
template <class Real>
Vector3<Real> operator* (Real scalar, const Vector3<Real>& rVector);

#ifdef WIRE_WII
#include "Wii/WireVector3Wii.inl"
#else
#include "WireVector3.inl"
#endif

typedef Vector3<Float> Vector3F;

}

#endif
