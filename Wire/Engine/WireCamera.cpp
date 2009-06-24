#include "WireCamera.h"

using namespace Wire;

//-------------------------------------------------------------------------
Camera::Camera()
{
}

//-------------------------------------------------------------------------
Camera::~Camera()
{
}

//-------------------------------------------------------------------------
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

//-------------------------------------------------------------------------
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
