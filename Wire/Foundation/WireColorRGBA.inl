// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline ColorRGBA::operator Float* ()
{
	return mTuple;
}

//----------------------------------------------------------------------------
inline ColorRGBA::operator const Float* () const
{
	return mTuple;
}

//----------------------------------------------------------------------------
inline Float ColorRGBA::R() const
{
	return mTuple[0];
}

//----------------------------------------------------------------------------
inline Float& ColorRGBA::R()
{
	return mTuple[0];
}

//----------------------------------------------------------------------------
inline Float ColorRGBA::G() const
{
	return mTuple[1];
}

//----------------------------------------------------------------------------
inline Float& ColorRGBA::G()
{
	return mTuple[1];
}

//----------------------------------------------------------------------------
inline Float ColorRGBA::B() const
{
	return mTuple[2];
}

//----------------------------------------------------------------------------
inline Float& ColorRGBA::B()
{
	return mTuple[2];
}

//----------------------------------------------------------------------------
inline Float ColorRGBA::A() const
{
	return mTuple[3];
}

//----------------------------------------------------------------------------
inline Float& ColorRGBA::A()
{
	return mTuple[3];
}

//----------------------------------------------------------------------------
inline ColorRGBA ColorRGBA::operator* (Float scalar) const
{
	return ColorRGBA(
		scalar * mTuple[0],
		scalar * mTuple[1],
		scalar * mTuple[2],
		scalar * mTuple[3]);
}

//----------------------------------------------------------------------------
inline ColorRGBA& ColorRGBA::operator*= (Float scalar)
{
	mTuple[0] *= scalar;
	mTuple[1] *= scalar;
	mTuple[2] *= scalar;
	mTuple[3] *= scalar;
	return *this;
}

//----------------------------------------------------------------------------
inline ColorRGBA& ColorRGBA::operator+= (const ColorRGBA& rCol)
{
	mTuple[0] += rCol[0];
	mTuple[1] += rCol[1];
	mTuple[2] += rCol[2];
	mTuple[3] += rCol[3];
	return *this;
}

//----------------------------------------------------------------------------
inline ColorRGBA operator* (Float scalar, const ColorRGBA& rCol)
{
	return ColorRGBA(
		scalar * rCol[0],
		scalar * rCol[1],
		scalar * rCol[2],
		scalar * rCol[3]);
}
