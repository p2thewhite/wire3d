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

	mWorldToCamera = Matrix34f(
		right.X(),	right.Y(),	right.Z(),	-(right.Dot(cameraPosition)),
		up.X(),		up.Y(),		up.Z(),		-(up.Dot(cameraPosition)),
		look.X(),	look.Y(),	look.Z(),	-(look.Dot(cameraPosition)));
}
