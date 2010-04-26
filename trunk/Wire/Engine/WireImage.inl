//----------------------------------------------------------------------------
inline UChar* Image::GetData() const
{
	return mpData;
}

//----------------------------------------------------------------------------
inline Image::FormatMode Image::GetFormat() const
{
	return mFormat;
}

//----------------------------------------------------------------------------
inline UInt Image::GetQuantity(UInt level) const
{
	if (level == 0)
	{
		return mBound[0] * mBound[1];
	}
	else
	{
		return GetBound(0, level) * GetBound(1, level);
	}
}

//----------------------------------------------------------------------------
inline Bool Image::HasMipmaps() const
{
	return mHasMipmaps;
}

//----------------------------------------------------------------------------
inline Bool Image::IsPowerOfTwo(UInt value) const
{
	return ((value & (value-1)) == 0);
}

//----------------------------------------------------------------------------
inline UInt Image::GetBytesPerPixel() const
{
	return GetBytesPerPixel(mFormat);
}

//----------------------------------------------------------------------------
inline UInt Image::GetBytesPerPixel(FormatMode format)
{
	return sImageBpp[format];
}

//----------------------------------------------------------------------------
inline void Image::RGB888ToRGB565(UChar* pSrc888, UChar* pDst565)
{
	UShort rgb565 = static_cast<UShort>(*pSrc888++);		// red
	rgb565 &= 0xF8;
	rgb565 = rgb565 << 8;

	UShort component = static_cast<UShort>(*pSrc888++);		// green
	component &= 0xFC;
	rgb565 |= component << 3;

	component = static_cast<UShort>(*pSrc888);				// blue
	component &= 0xF8;
	rgb565 |= component >> 3;

	*pDst565++ = static_cast<UChar>(rgb565 >> 8);
	*pDst565 = static_cast<UChar>(rgb565);
}

//----------------------------------------------------------------------------
inline void Image::RGBA8888ToRGBA4444(UChar* pSrc8888, UChar* pDst4444)
{
	UShort rgba4444 = static_cast<UShort>(*pSrc8888++);		// red
	rgba4444 &= 0xF0;
	rgba4444 = rgba4444 << 8;

	UShort component = static_cast<UShort>(*pSrc8888++);	// green
	component &= 0xF0;
	rgba4444 |= component << 4;

	component = static_cast<UShort>(*pSrc8888++);			// blue
	component &= 0xF0;
	rgba4444 |= component;

	component = static_cast<UShort>(*pSrc8888);				// alpha
	rgba4444 |= component >> 4;

	*pDst4444++ = static_cast<UChar>(rgba4444 >> 8);
	*pDst4444 = static_cast<UChar>(rgba4444);
}

//----------------------------------------------------------------------------
inline void Image::RGB565ToRGB888(UChar* pSrc565, UChar* pDst888)
{
	*pDst888++ = *pSrc565 & 0xF8;
	UChar gHigh = (*pSrc565++ & 0x07) << 5;
	UChar gLow = (*pSrc565 & 0xE0) >> 3;
	*pDst888++ = gHigh | gLow;
	*pDst888++ = (*pSrc565 & 0x1F) << 3;
}

//----------------------------------------------------------------------------
inline void Image::RGBA4444ToRGBA8888(UChar* pSrc4444, UChar* pDst8888)
{
	*pDst8888++ = *pSrc4444 & 0xF0;
	*pDst8888++ = (*pSrc4444++ & 0x0F) << 4;
	*pDst8888++ = *pSrc4444 & 0xF0;
	*pDst8888++ = (*pSrc4444 & 0x0F) << 4;
}
