// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void PdrVertexAttributes::Enable(Renderer* pRenderer)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->SetVertexDeclaration(mpDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
inline void PdrVertexAttributes::Disable(Renderer*)
{
}

//----------------------------------------------------------------------------
inline UInt PdrVertexAttributes::GetVertexSize() const
{
	return mVertexSize;
}
