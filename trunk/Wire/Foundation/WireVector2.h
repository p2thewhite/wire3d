#pragma once
#ifndef WIREVECTOR2_H
#define WIREVECTOR2_H

#include "WireTypes.h"
#include "WireMath.h"

namespace Wire
{

template <class Real>
class Vector2
{
public:
	// construction
	Vector2();  // uninitialized
	Vector2(Real x, Real y);

	// coordinate access
	inline operator Real* ();
	inline operator const Real* () const;
	inline Real X() const;
	inline Real X();
	inline Real Y() const;
	inline Real Y();

	// arithmetic operations
	inline Vector2 operator+ (const Vector2& rVector);
	inline Vector2 operator- (const Vector2& rVector);
	inline Vector2 operator* (Real scalar);
	inline Vector2 operator/ (Real scalar);
	inline Vector2 operator- () const;

	// vector operations
	inline Real Length() const;
	inline Real SquaredLength() const;

private:
	Real mTuple[2];
};

#include "WireVector2.inl"

typedef Vector2<Float> Vector2F;

}

#endif
