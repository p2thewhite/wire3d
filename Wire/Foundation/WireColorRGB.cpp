// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

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

//----------------------------------------------------------------------------
ColorRGB ColorRGB::operator* (Float scalar) const
{
	return ColorRGB(
		scalar * mTuple[0],
		scalar * mTuple[1],
		scalar * mTuple[2]);
}

//----------------------------------------------------------------------------
ColorRGB& ColorRGB::operator*= (Float scalar)
{
	mTuple[0] *= scalar;
	mTuple[1] *= scalar;
	mTuple[2] *= scalar;
	return *this;
}

//----------------------------------------------------------------------------
ColorRGB operator* (Float scalar, const ColorRGB& rCol)
{
	return ColorRGB(
		scalar * rCol.R(),
		scalar * rCol.G(),
		scalar * rCol.B());
}
