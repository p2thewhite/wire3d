#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetState(MaterialState* pState)
{
	mspStates[GlobalState::MATERIAL] = pState;

	GXColor material;
	material.r = pState->Ambient.R();
	material.g = pState->Ambient.G();
	material.b = pState->Ambient.B();
	material.a = pState->Ambient.A();

//	GXSetChanMatColor(GX_COLOR0A0, material);
}
