// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void* PdrVertexBuffer::Lock(Buffer::LockingMode)
{
	return mpBuffer;
}

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::Unlock()
{
	DCStoreRange(mpBuffer, mBufferSize);
	GXInvalidateVtxCache();
}

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::SetBuffer(Renderer*, UInt vertexSize)
{
	WIRE_ASSERT(vertexSize > 0);
	WIRE_ASSERT(mpPdrVertexAttributes);

	const TArray<PdrVertexAttributes::VertexElement>& rDeclaration =
		mpPdrVertexAttributes->GetDeclaration();

	for (UInt i = 0; i < rDeclaration.GetQuantity(); i++)
	{
		void* pArray = reinterpret_cast<void*>((rDeclaration[i].Offset +
			reinterpret_cast<UInt>(mpBuffer)));
		GXSetArray(rDeclaration[i].Attr, pArray, vertexSize);
	}
}

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::SetDeclaration(Renderer*)
{
	WIRE_ASSERT(mpPdrVertexAttributes);
	mpPdrVertexAttributes->Enable(NULL);
}

//----------------------------------------------------------------------------
inline const TArray<PdrVertexAttributes::VertexElement>&
PdrVertexBuffer::GetDeclaration() const
{
	WIRE_ASSERT(mpPdrVertexAttributes);
	return mpPdrVertexAttributes->GetDeclaration();
}

//----------------------------------------------------------------------------
inline UInt PdrVertexBuffer::GetBufferSize() const
{
	return mBufferSize;
}

//----------------------------------------------------------------------------
inline UInt PdrVertexBuffer::GetVertexSize() const
{
	WIRE_ASSERT(mpPdrVertexAttributes);
	return mpPdrVertexAttributes->GetVertexSize();
}
