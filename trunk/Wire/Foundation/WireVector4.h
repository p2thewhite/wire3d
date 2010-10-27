#pragma once
#ifndef WIREVECTOR4_H
#define WIREVECTOR4_H

#include "WireTypes.h"
#include "WireMath.h"

namespace Wire
{

template <class Real>
class Vector4
{
public:
	// construction
	Vector4();  // uninitialized
	Vector4(Real x, Real y, Real z, Real w);

	// coordinate access
	inline operator Real* ();
	inline operator const Real* () const;
	inline Real X() const;
	inline Real& X();
	inline Real Y() const;
	inline Real& Y();
	inline Real Z() const;
	inline Real& Z();
	inline Real W() const;
	inline Real& W();

 	// assignment
 	inline Vector4& operator= (const Vector4& rVector);
 
	// comparison
	Bool operator== (const Vector4& rVector) const;
	Bool operator!= (const Vector4& rVector) const;

 	// arithmetic operations
	inline Vector4 operator+ (const Vector4& rVector) const;
	inline Vector4 operator- (const Vector4& rVector) const;
	inline Vector4 operator* (Real scalar) const;
	inline Vector4 operator/ (Real scalar) const;
	inline Vector4 operator- () const;
 
	// vector operations
	inline Real Length() const;
	inline Real SquaredLength() const;
	inline Real Dot(const Vector4& rVector) const;
	inline Real Normalize();

	/*WIRE_FOUNDATION_ITEM*/ static const Vector4 ZERO;    // (0,0,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector4 UNIT_X;  // (1,0,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector4 UNIT_Y;  // (0,1,0,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector4 UNIT_Z;  // (0,0,1,0)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector4 UNIT_W;  // (0,0,0,1)
	/*WIRE_FOUNDATION_ITEM*/ static const Vector4 ONE;     // (1,1,1,1)

private:
	Real mTuple[4];
};

// arithmetic operations
template <class Real>
Vector4<Real> operator* (Real scalar, const Vector4<Real>& rVector);

#include "WireVector4.inl"

typedef Vector4<Float> Vector4F;
typedef Vector4<Double> Vector4D;

}

#endif
