#include "WireColorRGB.h"

using namespace Wire;

const ColorRGB ColorRGB::BLACK(0.0F, 0.0F, 0.0F);
const ColorRGB ColorRGB::WHITE(1.0F, 1.0F, 1.0F);
const ColorRGB ColorRGB::RED(1.0F, 0.0F, 0.0F);
const ColorRGB ColorRGB::GREEN(0.0F, 1.0F, 0.0F);
const ColorRGB ColorRGB::BLUE(0.0F, 0.0F, 1.0F);

//----------------------------------------------------------------------------
ColorRGB::ColorRGB()
{
	mTuple[0] = 0.0F;
	mTuple[1] = 0.0F;
	mTuple[2] = 0.0F;
}

//----------------------------------------------------------------------------
ColorRGB::ColorRGB(Float red, Float green, Float blue)
{
	mTuple[0] = red;
	mTuple[1] = green;
	mTuple[2] = blue;
}
