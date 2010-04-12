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
inline UInt Image::GetQuantity() const
{
	return mBound[0] * mBound[1];
}

//----------------------------------------------------------------------------
inline UInt Image::GetBound(UInt i) const
{
	WIRE_ASSERT(0 <= i && i < 2);
	return mBound[i];
}

//----------------------------------------------------------------------------
inline Bool Image::IsPowerOfTwo(UInt value) const
{
	return ((value & (value-1)) == 0);
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
