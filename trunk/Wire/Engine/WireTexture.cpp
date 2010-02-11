#include "WireTexture.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Texture, Object);

//----------------------------------------------------------------------------
Texture::Texture(Image* pImage)
	:
	mspImage(pImage),
	mFilterType(FT_LINEAR),
	mAnisotropy(1.0F)
{
	mWarpType[0] = WT_CLAMP;
	mWarpType[1] = WT_CLAMP;
}

//----------------------------------------------------------------------------
Texture::~Texture()
{
	// Inform all renderers using this texture that it is being destroyed.
	// This allows the renderer to free up any associated resources.
	Release();
}
