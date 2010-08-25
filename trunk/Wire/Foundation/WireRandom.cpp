#include "WireRandom.h"

#include "WireSystem.h"

using namespace Wire;

//----------------------------------------------------------------------------
Random::Random(UInt seed)
{
	Init(seed);
}

//----------------------------------------------------------------------------
UInt Random::Get()
{
	ULongLong c = 2111111111*x[3] + 1492 * x[2] + 1776 * x[1] + 5115 * x[0] +
	  x[4];

	x[4] = (c>>32) & 0xFFFFFFFF;
	x[3] = x[2];
	x[2] = x[1];
	x[1] = x[0];
	x[0] = c & 0xFFFFFFFF;

	return static_cast<UInt>(x[0]);
}

//----------------------------------------------------------------------------
Float Random::GetFloat()
{
	return static_cast<Float>(Get()) / System::MaxUInt;
}

//----------------------------------------------------------------------------
Double Random::GetDouble()
{
	return static_cast<Double>(Get()) / System::MaxUInt;
}

//----------------------------------------------------------------------------
void Random::Init(UInt seed)
{
	UInt s = seed;

	// make random numbers and put them into the buffer
	for (UInt i = 0; i < 5; i++)
	{
		s = s * 29943829 - 1;
		x[i] = s;
	}

	// randomize some more
	for (UInt i = 0; i < 19; i++)
	{
		Get();
	}
}
