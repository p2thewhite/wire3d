#include "WireDx9Renderer.h"

using namespace Wire;

DWORD Dx9Renderer::msZBufferCompare[ZBufferState::CF_QUANTITY] = 
{
	D3DCMP_NEVER,           // ZBufferState::CF_NEVER
	D3DCMP_LESS,            // ZBufferState::CF_LESS
	D3DCMP_EQUAL,           // ZBufferState::CF_EQUAL
	D3DCMP_LESSEQUAL,       // ZBufferState::CF_LEQUAL
	D3DCMP_GREATER,         // ZBufferState::CF_GREATER
	D3DCMP_NOTEQUAL,        // ZBufferState::CF_NOTEQUAL
	D3DCMP_GREATEREQUAL,    // ZBufferState::CF_GEQUAL
	D3DCMP_ALWAYS,          // ZBufferState::CF_ALWAYS
};

//----------------------------------------------------------------------------
void Dx9Renderer::SetZBufferState(ZBufferState* pState)
{
	Renderer::SetZBufferState(pState);

	if (pState->Enabled)
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_ZFUNC,
			msZBufferCompare[pState->Compare]);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}

	if (pState->Writable)
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
}
