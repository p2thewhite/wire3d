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
		FM_RGB565,
		FM_RGBA4444,
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
	UInt GetBytesPerPixel() const;
	static UInt GetBytesPerPixel(FormatMode format);
	void GenerateMipmaps();

	static void RGB888ToRGB565(UChar* pSrc888, UChar* pDst565);
	static void RGBA8888ToRGBA4444(UChar* pRGBA8888, UChar* pDst4444);
	static void RGB565ToRGB888(UChar* pSrc565, UChar* pDst888);
	static void RGBA4444ToRGBA8888(UChar* pSrc4444, UChar* pDst8888);

private:
	Bool IsPowerOfTwo(UInt value) const;
	void GenerateMipmap(UChar* pSrc, UChar* pDst, UInt width, UInt height);

	FormatMode mFormat;
	UChar* mpData;
	UInt mBound[2];
};

typedef Pointer<Image> ImagePtr;
#include "WireImage.inl"

}

#endif
