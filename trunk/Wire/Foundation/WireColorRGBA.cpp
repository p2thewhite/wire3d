#include "WireColorRGBA.h"

using namespace Wire;

const ColorRGBA ColorRGBA::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const ColorRGBA ColorRGBA::WHITE(1.0f, 1.0f, 1.0f, 1.0f);

//-------------------------------------------------------------------------
ColorRGBA::ColorRGBA()
{
	mTuple[0] = 0.0f;
	mTuple[1] = 0.0f;
	mTuple[2] = 0.0f;
	mTuple[3] = 0.0f;
}

//-------------------------------------------------------------------------
ColorRGBA::ColorRGBA(Float red, Float green, Float blue, Float alpha)
{
	mTuple[0] = red;
	mTuple[1] = green;
	mTuple[2] = blue;
	mTuple[3] = alpha;
}
