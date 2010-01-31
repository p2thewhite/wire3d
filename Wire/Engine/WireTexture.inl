//----------------------------------------------------------------------------
inline Image* Texture::GetImage()
{
	return mspImage;
}

//----------------------------------------------------------------------------
inline const Image* Texture::GetImage() const
{
	return mspImage;
}

//----------------------------------------------------------------------------
inline void Texture::SetFilterType(FilterType filterType)
{
	mFilterType = filterType;
}
//----------------------------------------------------------------------------
inline Texture::FilterType Texture::GetFilterType() const
{
	return mFilterType;
}

//----------------------------------------------------------------------------
inline void Texture::SetWrapType(UInt i, WrapType wrapType)
{
	WIRE_ASSERT(0 <= i && i < 2);
	mWarpType[i] = wrapType;
}
//----------------------------------------------------------------------------
inline Texture::WrapType Texture::GetWrapType(UInt i) const
{
	WIRE_ASSERT(0 <= i && i < 2);
	return mWarpType[i];
}

//----------------------------------------------------------------------------
inline void Texture::SetAnisotropyValue(Float anisotropy)
{
	mAnisotropy = anisotropy;
}
//----------------------------------------------------------------------------
inline Float Texture::GetAnisotropyValue() const
{
	return mAnisotropy;
}
