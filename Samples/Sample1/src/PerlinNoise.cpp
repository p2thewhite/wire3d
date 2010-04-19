#include "PerlinNoise.h"

#include "WireMath.h"

//----------------------------------------------------------------------------
PerlinNoise::PerlinNoise(Float persistance, Int octaves)
{
	mPersistance = persistance;
	mOctaves = octaves;
}

//----------------------------------------------------------------------------
PerlinNoise::~PerlinNoise ()
{
}

// These functions are from Greg Ward's recursive implementation of
// Perlin Noise in Graphics Gems II. 

//----------------------------------------------------------------------------
Float PerlinNoise::Noise(Int x)
{
	x = (x<<13) ^ x;
	return static_cast<Float>(1.0 - ( ( x * (x * x * 15731 + 789221) +
		1376312589) & 0x7fffffff) / 1073741824.0);
}

//----------------------------------------------------------------------------
void PerlinNoise::Rand3abcd(Int x, Int y, Int z, Float outv[4])
{
	outv[0] = Noise(67*x + 59*y + 71*z);
	outv[1] = Noise(73*x + 79*y + 83*z);
	outv[2] = Noise(89*x + 97*y + 101*z);
	outv[3] = Noise(103*x + 107*y + 109*z);
}

//----------------------------------------------------------------------------
Float PerlinNoise::Hermite(Float p0, Float p1, Float r0, Float r1, Float t)
{
  Float tt = t*t;

  return static_cast<Float>(p0*((2.0*t - 3.0)*tt + 1.0) +
          p1*(-2.0*t + 3.0)*tt +
          r0*((t-2.0)*t+1.0)*t +
          r1*(t-1.0)*tt);
}

//----------------------------------------------------------------------------
void PerlinNoise::Interpolate(Float f[4], Int i, Int n, Int xlim[3][2],
	Float xarg[3])
{
	Float f0[4], f1[4];

	if (n == 0)
	{
		Rand3abcd(xlim[0][i&1], xlim[1][(i>>1) & 1], xlim[2][i>>2], f);
		return;
	}
	n--;

	Interpolate(f0, i, n, xlim, xarg);
	Interpolate(f1, i | (1<<n), n, xlim, xarg);

	f[0] = (1.0f - xarg[n])*f0[0] + xarg[n]*f1[0];
	f[1] = (1.0f - xarg[n])*f0[1] + xarg[n]*f1[1];
	f[2] = (1.0f - xarg[n])*f0[2] + xarg[n]*f1[2];
	f[3] = Hermite(f0[3], f1[3], f0[n], f1[n], xarg[n]);
}

//----------------------------------------------------------------------------
Float PerlinNoise::InterpolateNoise(Float pos[3])
{
	Float noise[4];
	Float xarg[3];
	Int xlim[3][2];

	xlim[0][0] = Int(floor(pos[0]));
	xlim[1][0] = Int(floor(pos[1]));
	xlim[2][0] = Int(floor(pos[2]));

	xlim[0][1] = xlim[0][0] + 1;
	xlim[1][1] = xlim[1][0] + 1;
	xlim[2][1] = xlim[2][0] + 1;

	xarg[0] = pos[0] - xlim[0][0];
	xarg[1] = pos[1] - xlim[1][0];
	xarg[2] = pos[2] - xlim[2][0];

	Interpolate(noise, 0, 3, xlim, xarg);
	return noise[3];
}

//----------------------------------------------------------------------------
Float PerlinNoise::EvaluateAtPos(Float x, Float y, Float z)
{
	Float total = 0.0f;
	
	for(Int i = 0; i < mOctaves-1; i++)
	{
		Float pos[3] = {x, y, z};
		Float frequency = pow(2.0f, i);
		Float amplitude = pow(mPersistance, i);

		pos[0] *= frequency;
		pos[1] *= frequency;
		pos[2] *= frequency;
		total = total + InterpolateNoise (pos) * amplitude;
	}

	return total;
}
