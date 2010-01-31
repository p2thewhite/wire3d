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
inline UInt Image::GetBound(UInt i) const
{
	WIRE_ASSERT(0 <= i && i < 2);
	return mBound[i];
}
