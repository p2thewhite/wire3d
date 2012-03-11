// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline UInt PdrIndexBuffer::GetBufferSize() const
{
	return mBufferSize;
}

//----------------------------------------------------------------------------
inline UShort* PdrIndexBuffer::GetBuffer()
{
	return mpBuffer;
}

//----------------------------------------------------------------------------
inline const UShort* PdrIndexBuffer::GetBuffer() const
{
	return mpBuffer;
}

//----------------------------------------------------------------------------
inline TMap<UInt, PdrDisplayList*>& PdrIndexBuffer::GetDisplayLists()
{
	return mDisplayLists;
}
