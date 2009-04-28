//-------------------------------------------------------------------------
inline void Object::IncrementReferences()
{
	mReferences++;
}

//-------------------------------------------------------------------------
inline int Object::GetReferences () const
{
	return mReferences;
}
