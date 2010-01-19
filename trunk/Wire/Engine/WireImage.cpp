#include "WireImage.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Image, Object);

//----------------------------------------------------------------------------
Image::Image(FormatMode format, UInt width, UInt height, UChar* pData)
	:
	mFormat(format),
	mpData(pData),
	mWidth(width),
	mHeight(height)
{
}

//----------------------------------------------------------------------------
Image::~Image()
{
	WIRE_DELETE[] mpData;
}
