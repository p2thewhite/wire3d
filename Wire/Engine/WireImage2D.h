#pragma once
#ifndef WIREIMAGE_H
#define WIREIMAGE_H

#include "WireObject.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Image2D : public Object
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

	// Construction and destruction. Image2D accepts responsibility for
	// deleting the input array.

	Image2D(FormatMode format, UInt width, UInt height, UChar* pData,
		Bool createMipmaps = true);
	virtual ~Image2D();

	// Pointer to the image (i.e. mipmap level 0)
	UChar* GetData() const;
	FormatMode GetFormat() const;

	// Get number of pixels in this image
	UInt GetQuantity(UInt level = 0) const;
	UInt GetBound(UInt i, UInt level = 0) const;
	UInt GetBytesPerPixel() const;
	static UInt GetBytesPerPixel(FormatMode format);

	Bool HasMipmaps() const;
	void CreateMipmaps();
	UInt GetMipmapCount() const;
	UInt GetMipmapQuantity(UInt level) const;

	// Pointer to a mipmap level
	UChar* GetMipmap(UInt level) const;

	// Offset of a level in the mipmap pyramid
	UInt GetMipmapOffset(UInt level) const;

	// Number of pixels across all mipmap levels
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

typedef Pointer<Image2D> Image2DPtr;
#include "WireImage2D.inl"

}

#endif
