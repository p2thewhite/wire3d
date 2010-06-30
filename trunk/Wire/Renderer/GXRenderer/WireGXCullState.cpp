#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

UChar PdrRendererData::sCullType[CullState::CM_QUANTITY] = 
{
	GX_CULL_BACK,
	GX_CULL_FRONT
};

//----------------------------------------------------------------------------
void Renderer::SetCullState(CullState* pState)
{
	mspStates[GlobalState::CULL] = pState;

	if (pState->Enabled)
	{
		GXSetCullMode(PdrRendererData::sCullType[pState->CullFace]);
	}
	else
	{
		GXSetCullMode(GX_CULL_NONE);
	}
}
