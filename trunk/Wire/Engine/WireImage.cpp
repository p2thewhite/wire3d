#include "WireImage.h"

using namespace Wire;

//----------------------------------------------------------------------------
Image::Image(FormatMode format, UChar* pData)
{
 	mFormat = format;
 	mpData = pData;
}

//----------------------------------------------------------------------------
Image::~Image()
{
}
