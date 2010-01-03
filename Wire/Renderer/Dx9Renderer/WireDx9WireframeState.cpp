#include "WireDx9Renderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Dx9Renderer::SetWireframeState(WireframeState* pState)
{
	Renderer::SetWireframeState(pState);

    if (pState->Enabled)
    {
        msResult = mpD3DDevice->SetRenderState(D3DRS_FILLMODE,
			D3DFILL_WIREFRAME);
        WIRE_ASSERT(SUCCEEDED(msResult));
    }
    else
    {
        msResult = mpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        WIRE_ASSERT(SUCCEEDED(msResult));
    }
}
