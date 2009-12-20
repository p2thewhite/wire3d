#include "WireImage.h"

using namespace Wire;

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
