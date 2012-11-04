// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline VertexBuffer* Mesh::GetVertexBuffer(UInt streamIndex)
{
	WIRE_ASSERT(streamIndex < mVertexBuffers.GetQuantity());
	return mVertexBuffers[streamIndex];
}

//----------------------------------------------------------------------------
inline const VertexBuffer* Mesh::GetVertexBuffer(UInt streamIndex) const
{
	WIRE_ASSERT(streamIndex < mVertexBuffers.GetQuantity());
	return mVertexBuffers[streamIndex];
}

//----------------------------------------------------------------------------
inline TArray<Pointer<VertexBuffer> >& Mesh::GetVertexBuffers()
{
	return mVertexBuffers;
}

//----------------------------------------------------------------------------
inline const TArray<Pointer<VertexBuffer> >& Mesh::GetVertexBuffers() const
{
	return mVertexBuffers;
}

//----------------------------------------------------------------------------
inline IndexBuffer* Mesh::GetIndexBuffer()
{
	return mspIndexBuffer;
}

//----------------------------------------------------------------------------
inline const IndexBuffer* Mesh::GetIndexBuffer() const
{
	return mspIndexBuffer;
}

//----------------------------------------------------------------------------
inline BoundingVolume* Mesh::GetModelBound()
{
	return mspModelBound;
}

//----------------------------------------------------------------------------
inline const BoundingVolume* Mesh::GetModelBound() const
{
	return mspModelBound;
}

//----------------------------------------------------------------------------
inline UInt Mesh::GetStartIndex()
{
	return mStartIndex;
}

//----------------------------------------------------------------------------
inline UInt Mesh::GetIndexCount()
{
	return mIndexCount;
}

//----------------------------------------------------------------------------
inline Bool Mesh::IsDirty()
{
	return mIsDirty;
}
