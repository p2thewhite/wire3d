// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt String::GetLength() const
{
    return mLength;
}

//----------------------------------------------------------------------------
inline String::operator const Char* () const
{
    return mpText;
}
