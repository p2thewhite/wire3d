//----------------------------------------------------------------------------
// smart pointers
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

//----------------------------------------------------------------------------
// run-time type information
//----------------------------------------------------------------------------
inline const Rtti& Object::GetType() const
{
	return TYPE;
}

//----------------------------------------------------------------------------
inline Bool Object::IsExactly(const Rtti& rType) const
{
	return GetType().IsExactly(rType);
}

//----------------------------------------------------------------------------
inline Bool Object::IsDerived(const Rtti& rType) const
{
	return GetType().IsDerived(rType);
}

//----------------------------------------------------------------------------
inline Bool Object::IsExactlyTypeOf(const Object* pObj) const
{
	return pObj && GetType().IsExactly(pObj->GetType());
}

//----------------------------------------------------------------------------
inline Bool Object::IsDerivedTypeOf(const Object* pObj) const
{
	return pObj && GetType().IsDerived(pObj->GetType());
}

//----------------------------------------------------------------------------
template <class T>
inline T* StaticCast(Object* pObj)
{
	return static_cast<T*>(pObj);
}

//----------------------------------------------------------------------------
template <class T>
inline const T* StaticCast(const Object* pObj)
{
	return static_cast<const T*>(pObj);
}

//----------------------------------------------------------------------------
template <class T>
T* DynamicCast(Object* pObj)
{
	return pObj && pObj->IsDerived(T::TYPE) ? static_cast<T*>(pObj) : 0;
}

//----------------------------------------------------------------------------
template <class T>
const T* DynamicCast(const Object* pObj)
{
	return pObj && pObj->IsDerived(T::TYPE) ? static_cast<const T*>(pObj) : 0;
}
