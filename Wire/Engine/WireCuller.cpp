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
	Int plane = mPlaneQuantity - 1;
	UInt mask = 1 << plane;

	for (int i = 0; i < mPlaneQuantity; i++, plane--, mask >>= 1)
	{
		if (mPlaneState & mask)
		{
			int side = pkBound->WhichSide(mPlanes[plane]);

			if (side < 0)
			{
				// Object is on negative side. Cull it.
				return false;
			}

			if (side > 0)
			{
				// Object is on positive side of plane. There is no need to
				// compare subobjects against this plane, so mark it as
				// inactive.
				mPlaneState &= ~mask;
			}
		}
	}

	return true;
}

#include <string.h>

//----------------------------------------------------------------------------
void Culler::SetFrustum(const Float* pFrustum)
{
	WIRE_ASSERT(mpCamera);
	if (!mpCamera)
	{
		return;
	}

	// copy the frustum values
	size_t size = Camera::VF_QUANTITY * sizeof(Float);
	System::Memcpy(mFrustum, size, pFrustum, size);

	Float dMin2 = mFrustum[Camera::VF_DMIN] * mFrustum[Camera::VF_DMIN];
	Float rMin2 = mFrustum[Camera::VF_RMIN] * mFrustum[Camera::VF_RMIN];
	Float rMax2 = mFrustum[Camera::VF_RMAX] * mFrustum[Camera::VF_RMAX];
	Float uMin2 = mFrustum[Camera::VF_UMIN] * mFrustum[Camera::VF_UMIN];
	Float uMax2 = mFrustum[Camera::VF_UMAX] * mFrustum[Camera::VF_UMAX];

	// get the camera coordinate frame
	Vector3F location = mpCamera->GetLocation();
	Vector3F dVector = mpCamera->GetDVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F rVector = mpCamera->GetRVector();
	Float ddE = dVector.Dot(location);

	// update the near plane
	mPlanes[Camera::VF_DMIN].Normal = dVector;
	mPlanes[Camera::VF_DMIN].Constant = ddE + mFrustum[Camera::VF_DMIN];

	// update the far plane
	mPlanes[Camera::VF_DMAX].Normal = -dVector;
	mPlanes[Camera::VF_DMAX].Constant =	-(ddE + mFrustum[Camera::VF_DMAX]);

	// update the bottom plane
	Float invLength = 1.0F / MathF::Sqrt(dMin2 + uMin2);
	Float c0 = -mFrustum[Camera::VF_UMIN] * invLength;  // D component
	Float c1 = +mFrustum[Camera::VF_DMIN] * invLength;  // U component
	mPlanes[Camera::VF_UMIN].Normal = c0 * dVector + c1 * uVector;
	mPlanes[Camera::VF_UMIN].Constant =
		location.Dot(mPlanes[Camera::VF_UMIN].Normal);

	// update the top plane
	invLength = 1.0F / MathF::Sqrt(dMin2 + uMax2);
	c0 = +mFrustum[Camera::VF_UMAX] * invLength;  // D component
	c1 = -mFrustum[Camera::VF_DMIN] * invLength;  // U component
	mPlanes[Camera::VF_UMAX].Normal = c0 * dVector + c1 * uVector;
	mPlanes[Camera::VF_UMAX].Constant =
		location.Dot(mPlanes[Camera::VF_UMAX].Normal);

	// update the left plane
	invLength = 1.0F / MathF::Sqrt(dMin2 + rMin2);
	c0 = -mFrustum[Camera::VF_RMIN] * invLength;  // D component
	c1 = +mFrustum[Camera::VF_DMIN] * invLength;  // R component
	mPlanes[Camera::VF_RMIN].Normal = c0 * dVector + c1 * rVector;
	mPlanes[Camera::VF_RMIN].Constant =
		location.Dot(mPlanes[Camera::VF_RMIN].Normal);

	// update the right plane
	invLength = 1.0F / MathF::Sqrt(dMin2 + rMax2);
	c0 = +mFrustum[Camera::VF_RMAX] * invLength;  // D component
	c1 = -mFrustum[Camera::VF_DMIN] * invLength;  // R component
	mPlanes[Camera::VF_RMAX].Normal = c0 * dVector + c1 * rVector;
	mPlanes[Camera::VF_RMAX].Constant =
		location.Dot(mPlanes[Camera::VF_RMAX].Normal);

	// all planes are active initially
	mPlaneState = static_cast<UInt>(~0);
}
