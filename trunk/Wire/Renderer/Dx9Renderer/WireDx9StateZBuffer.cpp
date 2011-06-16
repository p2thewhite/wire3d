// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

DWORD PdrRendererData::sZBufferCompare[StateZBuffer::CF_QUANTITY] = 
{
	D3DCMP_NEVER,           // StateZBuffer::CF_NEVER
	D3DCMP_LESS,            // StateZBuffer::CF_LESS
	D3DCMP_EQUAL,           // StateZBuffer::CF_EQUAL
	D3DCMP_LESSEQUAL,       // StateZBuffer::CF_LEQUAL
	D3DCMP_GREATER,         // StateZBuffer::CF_GREATER
	D3DCMP_NOTEQUAL,        // StateZBuffer::CF_NOTEQUAL
	D3DCMP_GREATEREQUAL,    // StateZBuffer::CF_GEQUAL
	D3DCMP_ALWAYS,          // StateZBuffer::CF_ALWAYS
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateZBuffer* pState)
{
	mspStates[State::ZBUFFER] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	if (pState->Enabled)
	{
		hr = rDevice->SetRenderState(D3DRS_ZFUNC,
			PdrRendererData::sZBufferCompare[pState->Compare]);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	if (pState->Writable)
	{
		hr = rDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}
