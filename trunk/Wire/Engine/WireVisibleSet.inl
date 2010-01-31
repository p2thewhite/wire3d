//----------------------------------------------------------------------------
inline void VisibleSet::Clear()
{
	mVisible.SetQuantity(0, false);
}

//----------------------------------------------------------------------------
inline UInt VisibleSet::GetQuantity() const
{
	return mVisible.GetQuantity();
}

//----------------------------------------------------------------------------
inline VisibleObject* VisibleSet::GetVisible()
{
	return mVisible.GetArray();
}

//----------------------------------------------------------------------------
inline VisibleObject& VisibleSet::GetVisible(UInt i)
{
	WIRE_ASSERT(i < GetQuantity());
	return mVisible[i];
}
