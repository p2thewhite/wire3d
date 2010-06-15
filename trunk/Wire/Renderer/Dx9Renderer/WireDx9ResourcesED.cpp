#include "WireRenderer.h"

#include "WireDx9RendererData.h"
#include "WireDx9Resources.h"
#include "WireTexture2D.h"

using namespace Wire;

DWORD PdrRendererData::msTexMinFilter[Texture2D::FT_QUANTITY] =
{
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST_NEAREST
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST_LINEAR
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR_LINEAR
};

DWORD PdrRendererData::msTexMipFilter[Texture2D::FT_QUANTITY] =
{
	D3DTEXF_NONE,   // Texture2D::FT_NEAREST
	D3DTEXF_NONE,   // Texture2D::FT_LINEAR
	D3DTEXF_POINT,  // Texture2D::FT_NEAREST_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_NEAREST_LINEAR
	D3DTEXF_POINT,  // Texture2D::FT_LINEAR_NEAREST
	D3DTEXF_LINEAR, // Texture2D::FT_LINEAR_LINEAR
};

DWORD PdrRendererData::msTexWrapMode[Texture2D::WT_QUANTITY] =
{
	D3DTADDRESS_CLAMP,      // Texture2D::WT_CLAMP
	D3DTADDRESS_WRAP,       // Texture2D::WT_REPEAT
	D3DTADDRESS_MIRROR,     // Texture2D::WT_MIRRORED_REPEAT
};

//----------------------------------------------------------------------------
void Renderer::OnEnableIBuffer(ResourceIdentifier* pID)
{
	// Bind the current index buffer.
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	HRESULT hr;
	hr = mpData->mpD3DDevice->SetIndices(pResource->ID);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::OnDisableIBuffer(ResourceIdentifier*)
{
}

//----------------------------------------------------------------------------
void Renderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);
	IDirect3DDevice9*& rDevice = mpData->mpD3DDevice;
	HRESULT hr;
	hr = rDevice->SetStreamSource(0, pResource->ID, 0, pResource->
		VertexSize);
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetVertexDeclaration(pResource->Declaration);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::OnDisableVBuffer(ResourceIdentifier*)
{
}
