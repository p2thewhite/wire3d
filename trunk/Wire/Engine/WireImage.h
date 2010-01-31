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
		FM_RGBA8888,
		FM_QUANTITY
	};

	// Construction and destruction. Image accepts responsibility for
	// deleting the input array.

	// 2D image
	Image(FormatMode format, UInt width, UInt height, UChar* pData);
	virtual ~Image();

	UChar* GetData() const;
	FormatMode GetFormat() const;
	UInt GetQuantity() const;
	UInt GetBound(UInt i) const;

private:
	FormatMode mFormat;
	UChar* mpData;
	UInt mBound[2];
};

typedef Pointer<Image> ImagePtr;
#include "WireImage.inl"

}

#endif
