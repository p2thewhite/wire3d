// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

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
