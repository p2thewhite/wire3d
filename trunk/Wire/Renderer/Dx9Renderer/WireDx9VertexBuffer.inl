// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::Unlock()
{
	HRESULT hr;
	hr = mpBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::SetBuffer(Renderer* pRenderer)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->SetStreamSource(0, mpBuffer, 0, mVertexSize);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
inline void PdrVertexBuffer::SetDeclaration(Renderer* pRenderer)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->SetVertexDeclaration(mpDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));
}
