//----------------------------------------------------------------------------
inline void Culler::SetCamera(const Camera* pCamera)
{
	mpCamera = pCamera;
}
//----------------------------------------------------------------------------
inline const Camera* Culler::GetCamera() const
{
	return mpCamera;
}

//----------------------------------------------------------------------------
inline UInt Culler::GetPlaneState() const
{
	return mPlaneState;
}

//----------------------------------------------------------------------------
inline void Culler::SetPlaneState(UInt planeState)
{
	mPlaneState = planeState;
}

//----------------------------------------------------------------------------
inline VisibleSet& Culler::GetVisibleSet()
{
	return mVisible;
}

//----------------------------------------------------------------------------
inline void Culler::Insert(Spatial* pObject/*, Effect* pGlobalEffect*/)
{
	mVisible.Insert(pObject/*, pGlobalEffect*/);
}
