#include "WireDx9Renderer.h"

#include "WireTexture.h"
#include "WireDx9Resources.h"

using namespace Wire;

DWORD Dx9Renderer::msTexMinFilter[Texture::FT_QUANTITY] =
{
	D3DTEXF_POINT,  // Texture::FT_NEAREST
	D3DTEXF_LINEAR, // Texture::FT_LINEAR
	D3DTEXF_POINT,  // Texture::FT_NEAREST_NEAREST
	D3DTEXF_POINT,  // Texture::FT_NEAREST_LINEAR
	D3DTEXF_LINEAR, // Texture::FT_LINEAR_NEAREST
	D3DTEXF_LINEAR, // Texture::FT_LINEAR_LINEAR
};

DWORD Dx9Renderer::msTexMipFilter[Texture::FT_QUANTITY] =
{
	D3DTEXF_NONE,   // Texture::FT_NEAREST
	D3DTEXF_NONE,   // Texture::FT_LINEAR
	D3DTEXF_POINT,  // Texture::FT_NEAREST_NEAREST
	D3DTEXF_LINEAR, // Texture::FT_NEAREST_LINEAR
	D3DTEXF_POINT,  // Texture::FT_LINEAR_NEAREST
	D3DTEXF_LINEAR, // Texture::FT_LINEAR_LINEAR
};

DWORD Dx9Renderer::msTexWrapMode[Texture::WT_QUANTITY] =
{
	D3DTADDRESS_CLAMP,      // Texture::WT_CLAMP
	D3DTADDRESS_WRAP,       // Texture::WT_REPEAT
	D3DTADDRESS_MIRROR,     // Texture::WT_MIRRORED_REPEAT
};

//----------------------------------------------------------------------------
void Dx9Renderer::OnEnableIBuffer(ResourceIdentifier* pID)
{
	// Bind the current index buffer.
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	msResult = mpD3DDevice->SetIndices(pResource->ID);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnEnableVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);
	msResult = mpD3DDevice->SetStreamSource(0, pResource->ID, 0,
		pResource->VertexSize);
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpD3DDevice->SetVertexDeclaration(pResource->Declaration);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnEnableTexture(ResourceIdentifier* pID)
{
	TextureID* pResource = static_cast<TextureID*>(pID);
	Texture* pTexture = pResource->TextureObject;

	UInt unit = 0;

	// Anisotropic filtering value.
	Float anisotropy = pTexture->GetAnisotropyValue();
	if (1.0F < anisotropy && anisotropy <= msMaxAnisotropy)
	{
		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY,
			static_cast<DWORD>(anisotropy));
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MAXANISOTROPY,
			static_cast<DWORD>(1.0F));
		WIRE_ASSERT(SUCCEEDED(msResult));
	}

	// Set the filter mode.
	Texture::FilterType filterType = pTexture->GetFilterType();
	if (filterType == Texture::FT_NEAREST)
	{
		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER,
			D3DTEXF_POINT);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		if (1.0F < anisotropy && anisotropy <= msMaxAnisotropy)
		{
			msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER,
				D3DTEXF_ANISOTROPIC);
			WIRE_ASSERT(SUCCEEDED(msResult));
		}
		else
		{
			msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER,
				D3DTEXF_LINEAR);
			WIRE_ASSERT(SUCCEEDED(msResult));
		}
	}

	// Set the mipmap mode.
	if (1.0F < anisotropy && anisotropy <= msMaxAnisotropy)
	{
		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MINFILTER,
			D3DTEXF_ANISOTROPIC);
		WIRE_ASSERT(SUCCEEDED(msResult));

		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MIPFILTER,
			D3DTEXF_ANISOTROPIC);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MINFILTER,
			msTexMinFilter[filterType]);
		WIRE_ASSERT(SUCCEEDED(msResult));

		msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_MIPFILTER,
			msTexMipFilter[filterType]);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}

	// Set the border color (for clamp to border).
	const ColorRGBA borderColor(ColorRGBA::BLACK); //pTexture->GetBorderColor();
	msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_BORDERCOLOR,
		D3DCOLOR_COLORVALUE(borderColor.R(), borderColor.G(), borderColor.B(),
		borderColor.A()));
	WIRE_ASSERT(SUCCEEDED(msResult));

	msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_ADDRESSU,
		msTexWrapMode[pTexture->GetWrapType(0)]);
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpD3DDevice->SetSamplerState(unit, D3DSAMP_ADDRESSV,
		msTexWrapMode[pTexture->GetWrapType(1)]);
	WIRE_ASSERT(SUCCEEDED(msResult));

	msResult = mpD3DDevice->SetTexture(unit, pResource->ID);
	WIRE_ASSERT(SUCCEEDED(msResult));

	// TODO: introduce ApplyMode
	msResult = mpD3DDevice->SetTextureStageState(unit, D3DTSS_COLOROP, D3DTOP_MODULATE );
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpD3DDevice->SetTextureStageState(unit, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpD3DDevice->SetTextureStageState(unit, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpD3DDevice->SetTextureStageState(unit, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnDisableTexture(ResourceIdentifier*)
{
	UInt unit = 0;

	msResult = mpD3DDevice->SetTextureStageState(unit, D3DTSS_COLOROP,
		D3DTOP_DISABLE);
	WIRE_ASSERT(SUCCEEDED(msResult));

	msResult = mpD3DDevice->SetTexture(unit, 0);
	WIRE_ASSERT(SUCCEEDED(msResult));
}
