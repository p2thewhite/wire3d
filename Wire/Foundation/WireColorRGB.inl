// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline ColorRGB::operator Float* ()
{
	return mTuple;
}

//----------------------------------------------------------------------------
inline ColorRGB::operator const Float* () const
{
	return mTuple;
}

//----------------------------------------------------------------------------
inline Float ColorRGB::R() const
{
	return mTuple[0];
}

//----------------------------------------------------------------------------
inline Float& ColorRGB::R()
{
	return mTuple[0];
}

//----------------------------------------------------------------------------
inline Float ColorRGB::G() const
{
	return mTuple[1];
}

//----------------------------------------------------------------------------
inline Float& ColorRGB::G()
{
	return mTuple[1];
}

//----------------------------------------------------------------------------
inline Float ColorRGB::B() const
{
	return mTuple[2];
}

//----------------------------------------------------------------------------
inline Float& ColorRGB::B()
{
	return mTuple[2];
}

//----------------------------------------------------------------------------
inline ColorRGB ColorRGB::operator* (Float scalar) const
{
	return ColorRGB(
		scalar * mTuple[0],
		scalar * mTuple[1],
		scalar * mTuple[2]);
}

//----------------------------------------------------------------------------
inline ColorRGB& ColorRGB::operator*= (Float scalar)
{
	mTuple[0] *= scalar;
	mTuple[1] *= scalar;
	mTuple[2] *= scalar;
	return *this;
}

//----------------------------------------------------------------------------
inline ColorRGB& ColorRGB::operator+= (const ColorRGB& rCol)
{
	mTuple[0] += rCol[0];
	mTuple[1] += rCol[1];
	mTuple[2] += rCol[2];
	return *this;
}

//----------------------------------------------------------------------------
inline ColorRGB operator* (Float scalar, const ColorRGB& rCol)
{
	return ColorRGB(
		scalar * rCol[0],
		scalar * rCol[1],
		scalar * rCol[2]);
}
