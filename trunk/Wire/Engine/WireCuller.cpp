#include "WireCuller.h"

using namespace Wire;

//----------------------------------------------------------------------------
Culler::Culler(Int maxQuantity, Int growBy, const Camera* pCamera)
	:
	mpCamera(pCamera),
	mPlaneQuantity(6)
{
	// The data members mFrustum, mPlane, and mPlaneState are uninitialized.
	// They are initialized in the GetVisibleSet call.
}

//----------------------------------------------------------------------------
Culler::~Culler()
{
}

//----------------------------------------------------------------------------
bool Culler::IsVisible(const BoundingVolume* pkBound)
{
	// Start with the last pushed plane, which is potentially the most
	// restrictive plane.
// 	Int iP = mPlaneQuantity - 1;
// 	UInt uiMask = 1 << iP;
// 
// 	for (int i = 0; i < mPlaneQuantity; i++, iP--, uiMask >>= 1)
// 	{
// 		if (m_uiPlaneState & uiMask)
// 		{
// 			int iSide = pkBound->WhichSide(m_akPlane[iP]);
// 
// 			if (iSide < 0)
// 			{
// 				// Object is on negative side.  Cull it.
// 				return false;
// 			}
// 
// 			if (iSide > 0)
// 			{
// 				// Object is on positive side of plane.  There is no need to
// 				// compare subobjects against this plane, so mark it as
// 				// inactive.
// 				m_uiPlaneState &= ~uiMask;
// 			}
// 		}
// 	}
//TODO
	return true;
}
