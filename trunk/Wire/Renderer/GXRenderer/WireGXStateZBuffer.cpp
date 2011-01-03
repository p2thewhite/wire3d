#include "WireRenderer.h"

#include "WireGXRendererData.h"

using namespace Wire;

UChar PdrRendererData::sZBufferCompare[StateZBuffer::CF_QUANTITY] = 
{
	GX_NEVER,     // StateZBuffer::CF_NEVER
	GX_LESS,      // StateZBuffer::CF_LESS
	GX_EQUAL,     // StateZBuffer::CF_EQUAL
	GX_LEQUAL,    // StateZBuffer::CF_LEQUAL
	GX_GREATER,   // StateZBuffer::CF_GREATER
	GX_NEQUAL,    // StateZBuffer::CF_NOTEQUAL
	GX_GEQUAL,    // StateZBuffer::CF_GEQUAL
	GX_ALWAYS,    // StateZBuffer::CF_ALWAYS
};

//----------------------------------------------------------------------------
void Renderer::SetState(StateZBuffer* pState)
{
	mspStates[StateGlobal::ZBUFFER] = pState;

	UChar enable = pState->Enabled ? GX_TRUE : GX_FALSE;
	UChar writable = pState->Writable ? GX_TRUE : GX_FALSE;

	GXSetZMode(enable, PdrRendererData::sZBufferCompare[pState->Compare],
		writable);
}
