#pragma once
#ifndef WIREIMAGE_H
#define WIREIMAGE_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Image : public Object
{
	WIRE_DECLARE_RTTI;

public:
	enum FormatMode
	{
		FM_RGB888,
		FM_RGB8888,
		FM_QUANTITY
	};

	// Construction and destruction. Image accepts responsibility for
	// deleting the input array.

	// 2D image
	Image(FormatMode format, UInt width, UInt height, UChar* pData);

	virtual ~Image();

private:
	FormatMode mFormat;
	UChar* mpData;
	UInt mWidth;
	UInt mHeight;
};

typedef Pointer<Image> ImagePtr;

}

#endif
