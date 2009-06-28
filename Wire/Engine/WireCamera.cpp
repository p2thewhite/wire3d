#include "WireCamera.h"

using namespace Wire;

//----------------------------------------------------------------------------
Camera::Camera()
{
}

//----------------------------------------------------------------------------
Camera::~Camera()
{
}

//----------------------------------------------------------------------------
void Camera::LookAt(const Vector3f& cameraPosition,
	const Vector3f& cameraUp, const Vector3f& target)
{
	Vector3f look = cameraPosition - target;
	Vector3f right = cameraUp.Cross(look);
	Vector3f up = look.Cross(right);

	mView = Matrix34f(
		right.X(),	right.Y(),	right.Z(),	-(right.Dot(cameraPosition)),
		up.X(),		up.Y(),		up.Z(),		-(up.Dot(cameraPosition)),
		look.X(),	look.Y(),	look.Z(),	-(look.Dot(cameraPosition)));
}

//----------------------------------------------------------------------------
void Camera::SetFrustum(Float rMin, Float rMax, Float uMin, Float uMax,
	Float dMin, Float dMax)
{
	mFrustum[VF_DMIN] = dMin;
	mFrustum[VF_DMAX] = dMax;
	mFrustum[VF_UMIN] = uMin;
	mFrustum[VF_UMAX] = uMax;
	mFrustum[VF_RMIN] = rMin;
	mFrustum[VF_RMAX] = rMax;
}

//----------------------------------------------------------------------------
void Camera::SetFrustum(Float upFovDegrees, Float aspectRatio, Float dMin,
	Float dMax)
{
	Float halfAngleRadians = 0.5f * upFovDegrees * Mathf::DEG_TO_RAD;
	mFrustum[VF_UMAX] = dMin * Mathf::Tan(halfAngleRadians);
	mFrustum[VF_RMAX] = aspectRatio * mFrustum[VF_UMAX];
	mFrustum[VF_UMIN] = -mFrustum[VF_UMAX];
	mFrustum[VF_RMIN] = -mFrustum[VF_RMAX];
	mFrustum[VF_DMIN] = dMin;
	mFrustum[VF_DMAX] = dMax;
}

//----------------------------------------------------------------------------
void Camera::GetFrustum(Float& rRMin, Float& rRMax, Float& rUMin,
	Float& rUMax, Float& rDMin, Float& rDMax) const
{
	rDMin = mFrustum[VF_DMIN];
	rDMax = mFrustum[VF_DMAX];
	rUMin = mFrustum[VF_UMIN];
	rUMax = mFrustum[VF_UMAX];
	rRMin = mFrustum[VF_RMIN];
	rRMax = mFrustum[VF_RMAX];
}

//----------------------------------------------------------------------------
Bool Camera::GetFrustum(Float& rUpFovDegrees, Float& rAspectRatio,
	Float& rDMin, Float& rDMax) const
{
	if (mFrustum[VF_RMIN] == -mFrustum[VF_RMAX]
	&&  mFrustum[VF_UMIN] == -mFrustum[VF_UMAX])
	{
		Float tmp = mFrustum[VF_UMAX] / mFrustum[VF_DMIN];
		rUpFovDegrees = 2.0f * Mathf::ATan(tmp) * Mathf::RAD_TO_DEG;
		rAspectRatio = mFrustum[VF_RMAX] / mFrustum[VF_UMAX];
		rDMin = mFrustum[VF_DMIN];
		rDMax = mFrustum[VF_DMAX];
		return true;
	}

	return false;
}
