// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline const TArray<PdrVertexBuffer::VertexElement>&
PdrVertexBuffer::GetVertexElements() const
{
	return mElements;
}

//----------------------------------------------------------------------------
inline const UShort PdrVertexBuffer::GetVertexElementsId() const
{
	return mElementsId;
}

//----------------------------------------------------------------------------
inline Bool PdrVertexBuffer::HasNormals()
{
	return (mElementsId & 2) != 0;
}
