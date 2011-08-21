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

//----------------------------------------------------------------------------
void Renderer::SetState(StateWireframe* pState)
{
	WIRE_ASSERT(pState);
	mspStates[State::WIREFRAME] = pState;

	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
	HRESULT hr;
    if (pState->Enabled)
    {
        hr = rDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        WIRE_ASSERT(SUCCEEDED(hr));
    }
    else
    {
        hr = rDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        WIRE_ASSERT(SUCCEEDED(hr));
    }
}
