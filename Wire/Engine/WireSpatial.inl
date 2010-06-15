//----------------------------------------------------------------------------
inline void Spatial::SetParent(Spatial* pParent)
{
	mpParent = pParent;
}

//----------------------------------------------------------------------------
inline Spatial* Spatial::GetParent()
{
	return mpParent;
}

//----------------------------------------------------------------------------
inline UInt Spatial::GetGlobalStateQuantity() const
{
	return mGlobalStates.GetQuantity();
}

//----------------------------------------------------------------------------
inline GlobalState* Spatial::GetGlobalState(UInt i) const
{
	WIRE_ASSERT(0 <= i && i < mGlobalStates.GetQuantity());
	return mGlobalStates[i];
}

//----------------------------------------------------------------------------
inline void Spatial::DetachAllGlobalStates()
{
	mGlobalStates.RemoveAll();
}

//----------------------------------------------------------------------------
inline UInt Spatial::GetEffectQuantity() const
{
	return mEffects.GetQuantity();
}

//----------------------------------------------------------------------------
inline Effect* Spatial::GetEffect(UInt i) const
{
	WIRE_ASSERT(0 <= i && i < mEffects.GetQuantity());
	return StaticCast<Effect>(mEffects[i]);
}

//----------------------------------------------------------------------------
inline void Spatial::DetachAllEffects()
{
	mEffects.RemoveAll();
}