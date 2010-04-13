#include "WireImage.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Image, Object);

//----------------------------------------------------------------------------
Image::Image(FormatMode format, UInt width, UInt height, UChar* pData,
	Bool createMipmaps)
	:
	mFormat(format),
	mpData(pData),
	mHasMipmaps(createMipmaps)
{
	WIRE_ASSERT(IsPowerOfTwo(width) && width > 0);
	WIRE_ASSERT(IsPowerOfTwo(height) && height > 0);

	mBound[0] = width;
	mBound[1] = height;

	if (createMipmaps)
	{
		CreateMipmaps();
	}
}

//----------------------------------------------------------------------------
Image::~Image()
{
	WIRE_DELETE[] mpData;
}

//----------------------------------------------------------------------------
UInt Image::GetBytesPerPixel() const
{
	return GetBytesPerPixel(mFormat);
}

//----------------------------------------------------------------------------
UInt Image::GetBytesPerPixel(FormatMode format)
{
	switch (format)
	{
	case FM_RGB888:
		return 3;

	case FM_RGBA8888:
		return 4;

	case FM_RGB565:
	case FM_RGBA4444:
		return 2;
	default:
		break;
	}

	return 0;
}

//----------------------------------------------------------------------------
UInt Image::GetMipmapCount() const
{
	if (!mHasMipmaps)
	{
		return 0;
	}

	UInt mipmapCount = 0;
	UInt width = mBound[0];
	UInt height = mBound[1];

	while (width > 1 || height > 1)
	{
		mipmapCount++;
		width = width >> 1;
		height = height >> 1;
	}

	return mipmapCount;
}

//----------------------------------------------------------------------------
UChar* Image::GetMipmap(UInt i) const
{
	if (!mHasMipmaps || GetMipmapCount() < i)
	{
		return NULL;
	}

	UInt width = mBound[0];
	UInt height = mBound[1];
	UInt bpp = GetBytesPerPixel();
	UInt offset = 0;
	UInt level = 0;

	while ((width > 1 || height > 1) && level <= i)
	{
		offset += width * height * bpp;
		level++;
		width = width > 1 ? width >> 1 : width;
		height = height > 1 ? height >> 1 : height;
	}

	return mpData + offset;
}

//----------------------------------------------------------------------------
void Image::CreateMipmaps()
{
	UInt width = mBound[0];
	UInt height = mBound[1];
	UInt bpp = GetBytesPerPixel();
	UInt totalQuantity = 0;

	while (width > 1 || height > 1)
	{
		totalQuantity += width * height * bpp;
		width = width > 1 ? width >> 1 : width;
		height = height > 1 ? height >> 1 : height;
	}

	totalQuantity += bpp;

	UChar* pData = WIRE_NEW UChar[totalQuantity];
	UInt quantity = mBound[0]*mBound[1] * bpp;
	System::Memcpy(pData, quantity, mpData, quantity);
	WIRE_DELETE[] mpData;
	mpData = pData;

	width = mBound[0];
	height = mBound[1];

	while (width > 1 || height > 1)
	{
		UInt mipmapSize = width * height * bpp;
		CreateMipmap(pData, pData + mipmapSize, width, height);
		pData += mipmapSize;
		width = width > 1 ? width >> 1 : width;
		height = height > 1 ? height >> 1 : height;
	}
}

//----------------------------------------------------------------------------
void Image::CreateMipmap(UChar* pSrc, UChar* pDst, UInt width, UInt height)
{
	if (width == 1 && height == 1)
	{
		return;
	}

	if (width == 1 || height == 1)
	{
		CreateMipmap1(pSrc, pDst, width, height);
	}

	UInt bpp = GetBytesPerPixel();

	for (UInt y = 0; y < height; y+=2)
	{
		for (UInt x = 0; x < width; x+=2)
		{
			UChar box[4][4];
			UInt offset = (y*width+x)*bpp;

			switch (mFormat)
			{
			case FM_RGBA8888:
				box[0][0] = pSrc[offset];
				box[0][1] = pSrc[offset+1];
				box[0][2] = pSrc[offset+2];
				box[0][3] = pSrc[offset+3];

				box[1][0] = pSrc[offset+4];
				box[1][1] = pSrc[offset+5];
				box[1][2] = pSrc[offset+6];
				box[1][3] = pSrc[offset+7];

				offset += width*bpp;
				box[2][0] = pSrc[offset];
				box[2][1] = pSrc[offset+1];
				box[2][2] = pSrc[offset+2];
				box[2][3] = pSrc[offset+3];

				box[3][0] = pSrc[offset+4];
				box[3][1] = pSrc[offset+5];
				box[3][2] = pSrc[offset+6];
				box[3][3] = pSrc[offset+7];
				break;

			case FM_RGB888:
				box[0][0] = pSrc[offset];
				box[0][1] = pSrc[offset+1];
				box[0][2] = pSrc[offset+2];

				box[1][0] = pSrc[offset+3];
				box[1][1] = pSrc[offset+4];
				box[1][2] = pSrc[offset+5];

				offset += width*bpp;
				box[2][0] = pSrc[offset];
				box[2][1] = pSrc[offset+1];
				box[2][2] = pSrc[offset+2];

				box[3][0] = pSrc[offset+3];
				box[3][1] = pSrc[offset+4];
				box[3][2] = pSrc[offset+5];
				break;

			case FM_RGBA4444:
				RGBA4444ToRGBA8888(pSrc+offset, box[0]);
				RGBA4444ToRGBA8888(pSrc+offset+bpp, box[1]);
				offset += width*bpp;
				RGBA4444ToRGBA8888(pSrc+offset, box[2]);
				RGBA4444ToRGBA8888(pSrc+offset+bpp, box[3]);
				break;

			case FM_RGB565:
				RGB565ToRGB888(pSrc+offset, box[0]);
				RGB565ToRGB888(pSrc+offset+bpp, box[1]);
				offset += width*bpp;
				RGB565ToRGB888(pSrc+offset, box[2]);
				RGB565ToRGB888(pSrc+offset+bpp, box[3]);
				break;

			default:
				WIRE_ASSERT(false);
			}

			for (UInt j = 0; j < 4; j++)
			{
				UShort component = 0;
				for (UInt i = 0; i < 4; i++)
				{
					component = component + static_cast<UShort>(box[i][j]);
				}

				component = component >> 2;
				box[0][j] = static_cast<UChar>(component);
			}

			switch (mFormat)
			{
			case FM_RGB888:
				*pDst++ = box[0][0];
				*pDst++ = box[0][1];
				*pDst++ = box[0][2];
				break;

			case FM_RGBA8888:
				*pDst++ = box[0][0];
				*pDst++ = box[0][1];
				*pDst++ = box[0][2];
				*pDst++ = box[0][3];
				break;

			case FM_RGBA4444:
				RGBA8888ToRGBA4444(box[0], pDst);
				pDst += bpp;
				break;

			case FM_RGB565:
				RGB888ToRGB565(box[0], pDst);
				pDst += bpp;
				break;

			default:
				WIRE_ASSERT(false);
			}
		}
	}
}

//----------------------------------------------------------------------------
void Image::CreateMipmap1(UChar* pSrc, UChar* pDst, UInt width, UInt height)
{
	UInt bpp = GetBytesPerPixel();

	for (UInt y = 0; y < height; y+=2)
	{
		for (UInt x = 0; x < width; x+=2)
		{
			UChar box[4][4];
			UInt offset = (y*width+x)*bpp;
			UShort c = 0;

			switch (mFormat)
			{
			case FM_RGBA8888:
				box[c][0] = pSrc[offset];
				box[c][1] = pSrc[offset+1];
				box[c][2] = pSrc[offset+2];
				box[c++][3] = pSrc[offset+3];

				if (width > 1)
				{
					box[c][0] = pSrc[offset+4];
					box[c][1] = pSrc[offset+5];
					box[c][2] = pSrc[offset+6];
					box[c++][3] = pSrc[offset+7];
				}
				else if (height > 1)
				{
					offset += width*bpp;
					box[c][0] = pSrc[offset];
					box[c][1] = pSrc[offset+1];
					box[c][2] = pSrc[offset+2];
					box[c++][3] = pSrc[offset+3];
				}
				else
				{
					WIRE_ASSERT(false);
				}

				break;

			case FM_RGB888:
				box[c][0] = pSrc[offset];
				box[c][1] = pSrc[offset+1];
				box[c++][2] = pSrc[offset+2];

				if (width > 1)
				{
					box[c][0] = pSrc[offset+3];
					box[c][1] = pSrc[offset+4];
					box[c++][2] = pSrc[offset+5];
				}
				else if (height > 1)
				{
					offset += width*bpp;
					box[c][0] = pSrc[offset];
					box[c][1] = pSrc[offset+1];
					box[c++][2] = pSrc[offset+2];
				}
				else
				{
					WIRE_ASSERT(false);
				}

				break;

			case FM_RGBA4444:
				RGBA4444ToRGBA8888(pSrc+offset, box[c++]);
				if (width > 1)
				{
					RGBA4444ToRGBA8888(pSrc+offset+bpp, box[c++]);
				}
				else if (height > 1)
				{
					offset += width*bpp;
					RGBA4444ToRGBA8888(pSrc+offset, box[c++]);
				}
				else
				{
					WIRE_ASSERT(false);
				}

				break;

			case FM_RGB565:
				RGB565ToRGB888(pSrc+offset, box[c++]);
				if (width > 1)
				{
					RGB565ToRGB888(pSrc+offset+bpp, box[c++]);
				}
				else if (height > 1)
				{
					offset += width*bpp;
					RGB565ToRGB888(pSrc+offset, box[c++]);
				}
				else
				{
					WIRE_ASSERT(false);
				}

				break;

			default:
				WIRE_ASSERT(false);
			}

			for (UInt j = 0; j < 4; j++)
			{
				UShort component = 0;
				for (UInt i = 0; i < c; i++)
				{
					component = component + static_cast<UShort>(box[i][j]);
				}

				component = component / c;
				box[0][j] = static_cast<UChar>(component);
			}

			switch (mFormat)
			{
			case FM_RGB888:
				*pDst++ = box[0][0];
				*pDst++ = box[0][1];
				*pDst++ = box[0][2];
				break;

			case FM_RGBA8888:
				*pDst++ = box[0][0];
				*pDst++ = box[0][1];
				*pDst++ = box[0][2];
				*pDst++ = box[0][3];
				break;

			case FM_RGBA4444:
				RGBA8888ToRGBA4444(box[0], pDst);
				pDst += bpp;
				break;

			case FM_RGB565:
				RGB888ToRGB565(box[0], pDst);
				pDst += bpp;
				break;

			default:
				WIRE_ASSERT(false);
			}
		}
	}
}
