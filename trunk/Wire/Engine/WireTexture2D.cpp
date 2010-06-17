#include "WireTexture2D.h"

#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Texture2D, Object);

//----------------------------------------------------------------------------
Texture2D::Texture2D(Image2D* pImage)
	:
	mspImage(pImage),
	mFilterType(FT_LINEAR),
	mAnisotropy(1.0F)
{
	if (pImage->HasMipmaps())
	{
		mFilterType = FT_LINEAR_LINEAR;
	}

	mWarpType[0] = WT_CLAMP;
	mWarpType[1] = WT_CLAMP;
}

//----------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	// Inform all renderers using this texture that it is being destroyed.
	// This allows the renderer to free up any associated resources.
    Renderer::UnbindAll(this);
}
