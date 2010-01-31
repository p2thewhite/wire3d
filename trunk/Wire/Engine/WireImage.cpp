#include "WireImage.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Image, Object);

//----------------------------------------------------------------------------
Image::Image(FormatMode format, UInt width, UInt height, UChar* pData)
	:
	mFormat(format),
	mpData(pData)
{
	mBound[0] = width;
	mBound[1] = height;
}

//----------------------------------------------------------------------------
Image::~Image()
{
	WIRE_DELETE[] mpData;
}

//----------------------------------------------------------------------------
UInt Image::GetQuantity() const
{
	return mBound[0] * mBound[1];
}
