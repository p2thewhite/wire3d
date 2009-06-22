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
	inline Real X();
	inline Real Y() const;
	inline Real Y();
	inline Real Z() const;
	inline Real Z();

	// arithmetic operations
	inline Vector3 operator+ (const Vector3& rVector) const;
	inline Vector3 operator- (const Vector3& rVector) const;
	inline Vector3 operator* (Real scalar) const;
	inline Vector3 operator/ (Real scalar) const;
	inline Vector3 operator- () const;

	// vector operations
	inline Real Length() const;
	inline Real SquaredLength() const;
	inline Real Dot(const Vector3& rVector) const;

	inline Vector3 Cross(const Vector3& rVector) const;

	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 ZERO;    // (0,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 UNIT_X;  // (1,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 UNIT_Y;  // (0,1,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 UNIT_Z;  // (0,0,1)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector3 ONE;     // (1,1,1)

private:
#ifdef WIRE_WII
	Vector mTuple;
#else
	Real mTuple[3];
#endif
};

#ifdef WIRE_WII
#include "Wii/WireVector3Wii.inl"
#else
#include "WireVector3.inl"
#endif

typedef Vector3<Float> Vector3f;

}

#endif /* WIREVECTOR3_H */
