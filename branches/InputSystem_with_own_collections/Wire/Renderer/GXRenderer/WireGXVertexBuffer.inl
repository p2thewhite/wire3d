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
	for (UInt i = 0; i < mDeclaration.GetQuantity(); i++)
	{
		void* pArray = reinterpret_cast<void*>((mDeclaration[i].Offset +
			reinterpret_cast<UInt>(mpBuffer)));
		GXSetArray(mDeclaration[i].Attr, pArray, vertexSize);
	}
}

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::SetDeclaration(Renderer*)
{
	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();

	for (UInt i = 0; i < mDeclaration.GetQuantity(); i++)
	{
		GXSetVtxDesc(mDeclaration[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, mDeclaration[i].Attr, mDeclaration[i].
			CompCnt, mDeclaration[i].CompType, 0);
	}
}

//----------------------------------------------------------------------------
inline const TArray<PdrVertexBuffer::VertexElement>&
PdrVertexBuffer::GetDeclaration() const
{
	return mDeclaration;
}

//----------------------------------------------------------------------------
inline UInt PdrVertexBuffer::GetBufferSize() const
{
	return mBufferSize;
}
