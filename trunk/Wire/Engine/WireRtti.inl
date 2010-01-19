//----------------------------------------------------------------------------
inline Bool Rtti::IsExactly(const Rtti& rType) const
{
	return &rType == this;
}
