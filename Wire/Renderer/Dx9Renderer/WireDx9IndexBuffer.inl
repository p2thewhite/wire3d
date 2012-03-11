// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable(Renderer* pRenderer)
{
	HRESULT hr;
	hr = pRenderer->GetRendererData()->D3DDevice->SetIndices(mpBuffer);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable(Renderer* pRenderer)
{
	HRESULT hr;
	hr = pRenderer->GetRendererData()->D3DDevice->SetIndices(NULL);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
inline UInt PdrIndexBuffer::GetBufferSize() const
{
	return mBufferSize;
}

//----------------------------------------------------------------------------
inline UInt PdrIndexBuffer::GetIndexSize() const
{
	return mIndexSize;
}
