#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

//----------------------------------------------------------------------------
void Renderer::SetState(MaterialState* pState)
{
	mspStates[GlobalState::MATERIAL] = pState;

	mpData->Material.r = pState->Ambient.R();
	mpData->Material.g = pState->Ambient.G();
	mpData->Material.b = pState->Ambient.B();
	mpData->Material.a = pState->Ambient.A();
}
