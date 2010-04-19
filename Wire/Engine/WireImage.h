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
	Image(FormatMode format, UInt width, UInt height, UChar* pData,
		Bool createMipmaps = true);
	virtual ~Image();

	UChar* GetData() const;
	FormatMode GetFormat() const;
	UInt GetQuantity() const;
	UInt GetBound(UInt i, UInt level = 0) const;
	UInt GetBytesPerPixel() const;
	static UInt GetBytesPerPixel(FormatMode format);

	Bool HasMipmaps() const;
	void CreateMipmaps();
	UInt GetMipmapCount() const;
	UInt GetMipmapQuantity(UInt level) const;
	UChar* GetMipmap(UInt level) const;
	UInt GetMipmapOffset(UInt level) const;
	UInt GetTotalQuantity() const;

	static void RGB888ToRGB565(UChar* pSrc888, UChar* pDst565);
	static void RGBA8888ToRGBA4444(UChar* pRGBA8888, UChar* pDst4444);
	static void RGB565ToRGB888(UChar* pSrc565, UChar* pDst888);
	static void RGBA4444ToRGBA8888(UChar* pSrc4444, UChar* pDst8888);

private:
	Bool IsPowerOfTwo(UInt value) const;
	void CreateMipmap(UChar* pSrc, UChar* pDst, UInt width, UInt height);
	void CreateMipmap1(UChar* pSrc, UChar* pDst, UInt width, UInt height);

	static const UChar sImageBpp[];
	FormatMode mFormat;
	UChar* mpData;
	UInt mBound[2];
	Bool mHasMipmaps;
};

typedef Pointer<Image> ImagePtr;
#include "WireImage.inl"

}

#endif
