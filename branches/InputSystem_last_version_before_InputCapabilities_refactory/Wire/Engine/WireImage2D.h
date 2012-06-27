// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREIMAGE_H
#define WIREIMAGE_H

#include "WireObject.h"

namespace Wire
{

class Image2D : public Object
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
	inline UChar* GetData() const;
	inline FormatMode GetFormat() const;

	// Get number of pixels in this image
	inline UInt GetQuantity(UInt level = 0) const;
	UInt GetBound(UInt i, UInt level = 0) const;
	inline UInt GetBytesPerPixel() const;
	inline static UInt GetBytesPerPixel(FormatMode format);

	inline Bool HasAlpha() const;
	inline Bool HasMipmaps() const;
	void CreateMipmaps();
	UInt GetMipmapCount() const;
	UInt GetMipmapQuantity(UInt level) const;

	// Pointer to a mipmap level
	UChar* GetMipmap(UInt level) const;

	// Offset of a level in the mipmap pyramid
	UInt GetMipmapOffset(UInt level) const;

	// Number of pixels across all mipmap levels
	UInt GetTotalQuantity() const;

	inline static void RGB888ToRGB565(UChar* pSrc888, UChar* pDst565);
	inline static void RGBA8888ToRGBA4444(UChar* pRGBA8888, UChar* pDst4444);
	inline static void RGB565ToRGB888(UChar* pSrc565, UChar* pDst888);
	inline static void RGBA4444ToRGBA8888(UChar* pSrc4444, UChar* pDst8888);

private:
	inline Bool IsPowerOfTwo(UInt value) const;
	void CreateMipmap(UChar* pSrc, UChar* pDst, UInt width, UInt height);
	void CreateMipmap1(UChar* pSrc, UChar* pDst, UInt width, UInt height);

	static const UChar s_ImageBpp[];
	FormatMode mFormat;
	UChar* mpData;
	UInt mBound[2];
	Bool mHasMipmaps;
};

typedef Pointer<Image2D> Image2DPtr;
#include "WireImage2D.inl"

}

#endif
