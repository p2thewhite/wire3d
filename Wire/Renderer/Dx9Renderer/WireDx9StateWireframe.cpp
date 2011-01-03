#include "WireRenderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetState(StateWireframe* pState)
{
	mspStates[StateGlobal::WIREFRAME] = pState;

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
