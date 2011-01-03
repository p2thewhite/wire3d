#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

UChar PdrRendererData::sCullType[StateCull::CM_QUANTITY] = 
{
	GX_CULL_NONE,
	GX_CULL_BACK,
	GX_CULL_FRONT
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateCull* pState)
{
	mspStates[StateGlobal::CULL] = pState;

	if (pState->Enabled)
	{
		GXSetCullMode(PdrRendererData::sCullType[pState->CullFace]);
	}
	else
	{
		GXSetCullMode(GX_CULL_NONE);
	}
}
