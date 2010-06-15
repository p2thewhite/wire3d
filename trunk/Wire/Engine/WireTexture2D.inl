//----------------------------------------------------------------------------
inline Image2D* Texture2D::GetImage()
{
	return mspImage;
}

//----------------------------------------------------------------------------
inline const Image2D* Texture2D::GetImage() const
{
	return mspImage;
}

//----------------------------------------------------------------------------
inline void Texture2D::SetFilterType(FilterType filterType)
{
	mFilterType = filterType;
}
//----------------------------------------------------------------------------
inline Texture2D::FilterType Texture2D::GetFilterType() const
{
	return mFilterType;
}

//----------------------------------------------------------------------------
inline void Texture2D::SetWrapType(UInt i, WrapType wrapType)
{
	WIRE_ASSERT(0 <= i && i < 2);
	mWarpType[i] = wrapType;
}
//----------------------------------------------------------------------------
inline Texture2D::WrapType Texture2D::GetWrapType(UInt i) const
{
	WIRE_ASSERT(0 <= i && i < 2);
	return mWarpType[i];
}

//----------------------------------------------------------------------------
inline void Texture2D::SetAnisotropyValue(Float anisotropy)
{
	mAnisotropy = anisotropy;
}
//----------------------------------------------------------------------------
inline Float Texture2D::GetAnisotropyValue() const
{
	return mAnisotropy;
}
