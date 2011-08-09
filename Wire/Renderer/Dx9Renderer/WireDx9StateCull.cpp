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

const DWORD PdrRendererData::CULL_TYPE[StateCull::CM_QUANTITY] = 
{
	D3DCULL_NONE,
	D3DCULL_CCW,  // StateCull::CM_FRONT(front CCW -> cull backface CCW in DX)
	D3DCULL_CW,   // StateCull::CM_BACK (front CW -> cull backface CW in DX)
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateCull* pState)
{
	mspStates[State::CULL] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
	if (pState->Enabled)
	{
		hr = rDevice->SetRenderState(D3DRS_CULLMODE,
			PdrRendererData::CULL_TYPE[pState->CullFace]);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}
