#pragma once
#ifndef WIRENOISEPERLIN_H
#define WIRENOISEPERLIN_H

#include "WireMain.h"
#include "WireMath.h"

namespace Wire
{

// The base class contains the permutation table for all template instances.
class NoisePerlinBase
{
	WIRE_DECLARE_INITIALIZE;

protected:
	static const Int smPermutation[];
	static Int smP[];
};

WIRE_REGISTER_INITIALIZE(NoisePerlinBase);

//----------------------------------------------------------------------------
template <typename Real>
class NoisePerlin : public NoisePerlinBase
{
public:
	Real Noise(Real x, Real y);
	Real Noise(Real x, Real y, Real z);

private:
	Real Fade(Real t);
	Real Grad(Int hash, Real x, Real y, Real z);
	inline Real Lerp(Real t, Real a, Real b);
};

#include "WireNoisePerlin.inl"

typedef NoisePerlin<Float> NoisePerlinF;
typedef NoisePerlin<Double> NoisePerlinD;

}

#endif
