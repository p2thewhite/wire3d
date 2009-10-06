#pragma once
#ifndef WIREMATH_H
#define WIREMATH_H

#include "WireSystem.h"

namespace Wire
{

template <class Real>
class Math
{
public:
	static Real ATan(Real value);
	static Real Cos(Real value);
	static Real FAbs(Real value);
	static Real FMod(Real x, Real y);
	static Real Sin(Real value);
	static Real Sqrt(Real value);
	static Real Tan(Real value);

	// common constants
	/*WIRE_FOUNDATION_ITEM*/ static const Real MAX_REAL;
	/*WIRE_FOUNDATION_ITEM*/ static const Real PI;
	/*WIRE_FOUNDATION_ITEM*/ static const Real TWO_PI;
	/*WIRE_FOUNDATION_ITEM*/ static const Real HALF_PI;
	/*WIRE_FOUNDATION_ITEM*/ static const Real DEG_TO_RAD;
	/*WIRE_FOUNDATION_ITEM*/ static const Real RAD_TO_DEG;
	/*WIRE_FOUNDATION_ITEM*/ static const Real ZERO_TOLERANCE;
};

#include "WireMath.inl"

typedef Math<Float> MathF;
typedef Math<Double> MathD;

}

#endif
