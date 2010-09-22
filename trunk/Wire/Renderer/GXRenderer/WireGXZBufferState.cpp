#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

UChar PdrRendererData::sZBufferCompare[ZBufferState::CF_QUANTITY] = 
{
	GX_NEVER,     // ZBufferState::CF_NEVER
	GX_LESS,      // ZBufferState::CF_LESS
	GX_EQUAL,     // ZBufferState::CF_EQUAL
	GX_LEQUAL,    // ZBufferState::CF_LEQUAL
	GX_GREATER,   // ZBufferState::CF_GREATER
	GX_NEQUAL,    // ZBufferState::CF_NOTEQUAL
	GX_GEQUAL,    // ZBufferState::CF_GEQUAL
	GX_ALWAYS,    // ZBufferState::CF_ALWAYS
};

//----------------------------------------------------------------------------
void Renderer::SetState(ZBufferState* pState)
{
	mspStates[GlobalState::ZBUFFER] = pState;

	UChar enable = pState->Enabled ? GX_TRUE : GX_FALSE;
	UChar writable = pState->Writable ? GX_TRUE : GX_FALSE;

	GXSetZMode(enable, PdrRendererData::sZBufferCompare[pState->Compare],
		writable);
}
