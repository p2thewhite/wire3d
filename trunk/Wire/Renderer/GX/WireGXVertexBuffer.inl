// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::Disable(Renderer* pRenderer, UInt)
{
	pRenderer->GetRendererData()->PdrVBuffer = NULL;
}

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
inline UInt PdrVertexBuffer::GetBufferSize() const
{
	return mBufferSize;
}
