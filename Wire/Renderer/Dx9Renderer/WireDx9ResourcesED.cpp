#include "WireDx9Renderer.h"

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
void Dx9Renderer::OnEnableTexture (ResourceIdentifier* pkID)
{
	TextureID* pkResource = (TextureID*)pkID;
	Texture* pkTexture = pkResource->TextureObject;

	int iTextureUnit = 0;

	// Anisotropic filtering value.
	float fAnisotropy = pkTexture->GetAnisotropyValue();
	if (1.0F < fAnisotropy && fAnisotropy <= msMaxAnisotropy)
	{
		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MAXANISOTROPY, (DWORD)fAnisotropy);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MAXANISOTROPY, (DWORD)1.0f);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}

	// Set the filter mode.
	Texture::FilterType eFType = pkTexture->GetFilterType();
	if (eFType == Texture::FT_NEAREST)
	{
		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		if (1.0F < fAnisotropy && fAnisotropy <= msMaxAnisotropy)
		{
			msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
				D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			WIRE_ASSERT(SUCCEEDED(msResult));
		}
		else
		{
			msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
				D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			WIRE_ASSERT(SUCCEEDED(msResult));
		}
	}

	// Set the mipmap mode.
	if (1.0F < fAnisotropy && fAnisotropy <= msMaxAnisotropy)
	{
		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		WIRE_ASSERT(SUCCEEDED(msResult));

		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}
	else
	{
		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MINFILTER, msTexMinFilter[eFType]);
		WIRE_ASSERT(SUCCEEDED(msResult));

		msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
			D3DSAMP_MIPFILTER, msTexMipFilter[eFType]);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}

	// Set the border color (for clamp to border).
	const ColorRGBA rkBorderColor(ColorRGBA::BLACK); //pkTexture->GetBorderColor();
	msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
		D3DSAMP_BORDERCOLOR, D3DCOLOR_COLORVALUE(rkBorderColor.R(),
		rkBorderColor.G(), rkBorderColor.B(), rkBorderColor.A()));
	WIRE_ASSERT(SUCCEEDED(msResult));

	msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
		D3DSAMP_ADDRESSU, msTexWrapMode[pkTexture->GetWrapType(0)]);
	WIRE_ASSERT(SUCCEEDED(msResult));
	msResult = mpD3DDevice->SetSamplerState(iTextureUnit,
		D3DSAMP_ADDRESSV, msTexWrapMode[pkTexture->GetWrapType(1)]);
	WIRE_ASSERT(SUCCEEDED(msResult));

	msResult = mpD3DDevice->SetTexture(iTextureUnit, pkResource->ID);
	WIRE_ASSERT(SUCCEEDED(msResult));
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnDisableTexture(ResourceIdentifier*)
{
	int iTextureUnit = 0;

	msResult = mpD3DDevice->SetTextureStageState(iTextureUnit,
		D3DTSS_COLOROP, D3DTOP_DISABLE);
	WIRE_ASSERT(SUCCEEDED(msResult));

	msResult = mpD3DDevice->SetTexture(iTextureUnit, 0);
	WIRE_ASSERT(SUCCEEDED(msResult));
}
