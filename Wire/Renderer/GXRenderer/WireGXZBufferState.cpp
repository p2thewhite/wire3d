#include "WireGXRenderer.h"

using namespace Wire;

UChar GXRenderer::msZBufferCompare[ZBufferState::CF_QUANTITY] = 
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
void GXRenderer::SetZBufferState(ZBufferState* pState)
{
	Renderer::SetZBufferState(pState);

	UChar enable = pState->Enabled ? GX_TRUE : GX_FALSE;
	UChar writable = pState->Writable ? GX_TRUE : GX_FALSE;

	GXSetZMode(enable, msZBufferCompare[pState->Compare], writable);
}
