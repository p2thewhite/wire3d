// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireColorRGBA.h"

using namespace Wire;

const ColorRGBA ColorRGBA::BLACK(0.0F, 0.0F, 0.0F, 1.0F);
const ColorRGBA ColorRGBA::WHITE(1.0F, 1.0F, 1.0F, 1.0F);
const ColorRGBA ColorRGBA::RED(1.0F, 0.0F, 0.0F, 1.0F);
const ColorRGBA ColorRGBA::GREEN(0.0F, 1.0F, 0.0F, 1.0F);
const ColorRGBA ColorRGBA::BLUE(0.0F, 0.0F, 1.0F, 1.0F);

//----------------------------------------------------------------------------
ColorRGBA::ColorRGBA()
{
	mTuple[0] = 0.0F;
	mTuple[1] = 0.0F;
	mTuple[2] = 0.0F;
	mTuple[3] = 0.0F;
}

//----------------------------------------------------------------------------
ColorRGBA::ColorRGBA(Float red, Float green, Float blue, Float alpha)
{
	mTuple[0] = red;
	mTuple[1] = green;
	mTuple[2] = blue;
	mTuple[3] = alpha;
}

//----------------------------------------------------------------------------
ColorRGBA ColorRGBA::operator* (Float scalar) const
{
	return ColorRGBA(
		scalar * mTuple[0],
		scalar * mTuple[1],
		scalar * mTuple[2],
		scalar * mTuple[3]);
}

//----------------------------------------------------------------------------
ColorRGBA& ColorRGBA::operator*= (Float scalar)
{
	mTuple[0] *= scalar;
	mTuple[1] *= scalar;
	mTuple[2] *= scalar;
	mTuple[3] *= scalar;
	return *this;
}

//----------------------------------------------------------------------------
ColorRGBA operator* (Float scalar, const ColorRGBA& rCol)
{
	return ColorRGBA(
		scalar * rCol.R(),
		scalar * rCol.G(),
		scalar * rCol.B(),
		scalar * rCol.A());
}
