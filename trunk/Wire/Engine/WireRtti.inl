//----------------------------------------------------------------------------
inline const Char* Rtti::GetName() const
{
	return mpName;
}

//----------------------------------------------------------------------------
inline Bool Rtti::IsExactly(const Rtti& rType) const
{
	return &rType == this;
}
