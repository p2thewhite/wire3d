#pragma once
#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "WireTypes.h"

class PerlinNoise
{
public:
	PerlinNoise(Float persistance = 0.5, Int octaves = 32);
	~PerlinNoise();

	Float EvaluateAtPos(Float x, Float y = 0, Float z = 0);

private:
	static Float Noise(Int x);
	static void Rand3abcd(Int x, Int y, Int z, Float outv[4]);
	static Float Hermite(Float p0, Float p1, Float r0, Float r1, Float t);
	static void Interpolate(Float f[4], Int i, Int n, Int xlim[3][2],
		Float xarg[3]);
	static Float InterpolateNoise (Float pos[3]);

	Float mPersistance;
	Int mOctaves;
};

#endif
