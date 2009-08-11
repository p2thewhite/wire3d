#include "WireColorRGB.h"

using namespace Wire;

const ColorRGB ColorRGB::BLACK(0.0f, 0.0f, 0.0f);
const ColorRGB ColorRGB::WHITE(1.0f, 1.0f, 1.0f);
const ColorRGB ColorRGB::RED(1.0f, 0.0f, 0.0f);
const ColorRGB ColorRGB::GREEN(0.0f, 1.0f, 0.0f);
const ColorRGB ColorRGB::BLUE(0.0f, 0.0f, 1.0f);

//----------------------------------------------------------------------------
ColorRGB::ColorRGB()
{
	mTuple[0] = 0.0f;
	mTuple[1] = 0.0f;
	mTuple[2] = 0.0f;
}

//----------------------------------------------------------------------------
ColorRGB::ColorRGB(Float red, Float green, Float blue)
{
	mTuple[0] = red;
	mTuple[1] = green;
	mTuple[2] = blue;
}
