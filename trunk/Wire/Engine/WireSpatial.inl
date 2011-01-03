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
inline StateGlobal* Spatial::GetGlobalState(UInt i) const
{
	WIRE_ASSERT(i < mGlobalStates.GetQuantity());
	return mGlobalStates[i];
}

//----------------------------------------------------------------------------
inline void Spatial::DetachAllGlobalStates()
{
	mGlobalStates.RemoveAll();
}

//----------------------------------------------------------------------------
inline UInt Spatial::GetLightQuantity() const
{
	return mLights.GetQuantity();
}

//----------------------------------------------------------------------------
inline Light* Spatial::GetLight(UInt i) const
{
	WIRE_ASSERT(i < mLights.GetQuantity());
	return mLights[i];
}

//----------------------------------------------------------------------------
inline void Spatial::DetachLight(Light* pLight)
{
	mLights.Remove(pLight);
}

//----------------------------------------------------------------------------
inline void Spatial::DetachAllLights()
{
	mLights.RemoveAll();
}

//----------------------------------------------------------------------------
inline UInt Spatial::GetEffectQuantity() const
{
	return mEffects.GetQuantity();
}

//----------------------------------------------------------------------------
inline Effect* Spatial::GetEffect(UInt i) const
{
	WIRE_ASSERT(i < mEffects.GetQuantity());
	return mEffects[i];
}

//----------------------------------------------------------------------------
inline void Spatial::DetachEffect(Effect* pEffect)
{
	mEffects.Remove(pEffect);
}

//----------------------------------------------------------------------------
inline void Spatial::DetachAllEffects()
{
	mEffects.RemoveAll();
}
