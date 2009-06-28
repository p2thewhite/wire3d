//----------------------------------------------------------------------------
inline void Object::IncrementReferences()
{
	mReferences++;
}

//----------------------------------------------------------------------------
inline Int Object::GetReferences() const
{
	return mReferences;
}
