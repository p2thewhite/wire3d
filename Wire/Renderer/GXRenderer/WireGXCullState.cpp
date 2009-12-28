#include "WireGXRenderer.h"

using namespace Wire;

UChar GXRenderer::msCullType[CullState::CM_QUANTITY] = 
{
	GX_CULL_BACK,
	GX_CULL_FRONT
};

//----------------------------------------------------------------------------
void GXRenderer::SetCullState(CullState* pState)
{
	Renderer::SetCullState(pState);

	if (pState->Enabled)
	{
		GXSetCullMode(msCullType[pState->CullFace]);
	}
	else
	{
		GXSetCullMode(GX_CULL_NONE);
	}
}
