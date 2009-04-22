#pragma once
#ifndef WIREMATH_H
#define WIREMATH_H

#include "WireSystem.h"
#include "WireTypes.h"

namespace Wire
{

template <class Real>
class Math
{
public:
	static Real Cos(Real value);
	static Real Sin(Real value);
	static Real Sqrt(Real value);
	static Real FMod(Real x, Real y);
	
	// common constants
	// TODO: fix this nasty hack!
	/*WIRE_FOUNDATION_ITEM*/ static const Real MAX_REAL = 3.402823466e+38F;
};

#include "WireMath.inl"

typedef Math<Float> Mathf;
typedef Math<Double> Mathd;

}

#endif /* WIREMATH_H */
