// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

//----------------------------------------------------------------------------
inline void Culler::SetCamera(const Camera* pCamera)
{
	mpCamera = pCamera;
	SetFrustum(mpCamera->GetFrustum());
}

//----------------------------------------------------------------------------
inline const Camera* Culler::GetCamera() const
{
	return mpCamera;
}

//----------------------------------------------------------------------------
inline void Culler::SetPlaneState(UInt planeState)
{
	mPlaneState = planeState;
}

//----------------------------------------------------------------------------
inline UInt Culler::GetPlaneState() const
{
	return mPlaneState;
}

//----------------------------------------------------------------------------
inline VisibleSet& Culler::GetVisibleSet(UInt i)
{
	WIRE_ASSERT(i < mVisibleSets.GetQuantity());
	return mVisibleSets[i];
}

//----------------------------------------------------------------------------
inline TArray<VisibleSet>& Culler::GetVisibleSets()
{
	return mVisibleSets;
}

//----------------------------------------------------------------------------
inline void Culler::Insert(Spatial* pObject, Effect* pGlobalEffect)
{
	GetVisibleSet().Insert(pObject, pGlobalEffect);
}
