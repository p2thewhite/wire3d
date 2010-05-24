#include "WireDx9Renderer.h"

#include "WireDx9RendererData.h"

using namespace Wire;

DWORD PdrRendererData::msZBufferCompare[ZBufferState::CF_QUANTITY] = 
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
	mspStates[GlobalState::ZBUFFER] = pState;

	IDirect3DDevice9*& rDevice = mpData->mpD3DDevice;
	HRESULT hr;
	if (pState->Enabled)
	{
		hr = rDevice->SetRenderState(D3DRS_ZFUNC,
			PdrRendererData::msZBufferCompare[pState->Compare]);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		WIRE_ASSERT(SUCCEEDED(hr));
	}

	if (pState->Writable)
	{
		hr = rDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
	else
	{
		hr = rDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		WIRE_ASSERT(SUCCEEDED(hr));
	}
}
