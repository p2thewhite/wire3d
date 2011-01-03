#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetState(StateMaterial* pState)
{
	mspStates[StateGlobal::MATERIAL] = pState;

	GXColor material;
	material.r = static_cast<UChar>(pState->Ambient.R() * 255.0F);
	material.g = static_cast<UChar>(pState->Ambient.G() * 255.0F);
	material.b = static_cast<UChar>(pState->Ambient.B() * 255.0F);
	material.a = static_cast<UChar>(pState->Ambient.A() * 255.0F);

	GXSetChanMatColor(GX_COLOR0A0, material);
}
