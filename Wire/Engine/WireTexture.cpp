#include "WireTexture.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Texture, Object);

//----------------------------------------------------------------------------
Texture::Texture(Image* pImage)
	:
	mspImage(pImage),
	mFilterType(FT_LINEAR)
{
	mWarpType[0] = WT_CLAMP;
	mWarpType[1] = WT_CLAMP;
}

//----------------------------------------------------------------------------
Texture::~Texture()
{
}
