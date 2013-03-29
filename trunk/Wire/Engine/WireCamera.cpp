// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireCamera.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Camera, Object);

//----------------------------------------------------------------------------
Camera::Camera(Bool isPerspective)
	:
	mIsPerspective(isPerspective)
{
	SetFrustum(-0.5F, 0.5F, -0.4F, 0.4F, 1.0F, 100.0F);
	SetViewport(0.0F, 1.0F, 1.0F, 0.0F);
	SetFrame(Vector3F::ZERO, -Vector3F::UNIT_Z, Vector3F::UNIT_Y,
		Vector3F::UNIT_X);
}

//----------------------------------------------------------------------------
Camera::Camera(const Vector3F& location, const Vector3F& direction,
	const Vector3F& up, Bool isPerspective)
	:
	mIsPerspective(isPerspective)
{
	SetFrustum(-0.5F, 0.5F, -0.4F, 0.4F, 1.0F, 100.0F);
	SetViewport(0.0F, 1.0F, 1.0F, 0.0F);
	Vector3F right = direction.Cross(up);
	SetFrame(location, direction, up, right);
}

//----------------------------------------------------------------------------
Camera::~Camera()
{
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
	Float halfAngleRadians = 0.5F * upFovDegrees * MathF::DEG_TO_RAD;
	mFrustum[VF_UMAX] = dMin * MathF::Tan(halfAngleRadians);
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
		rUpFovDegrees = 2.0F * MathF::ATan(tmp) * MathF::RAD_TO_DEG;
		rAspectRatio = mFrustum[VF_RMAX] / mFrustum[VF_UMAX];
		rDMin = mFrustum[VF_DMIN];
		rDMax = mFrustum[VF_DMAX];
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------
void Camera::SetAspectRatio(Float aspectRation)
{
	Float fov;
	Float oldAspectRatio;
	Float near;
	Float far;

	GetFrustum(fov, oldAspectRatio, near, far);
	SetFrustum(fov, aspectRation, near, far);
}

//----------------------------------------------------------------------------
void Camera::SetAxes(const Vector3F& rDVector, const Vector3F& rUVector,
	const Vector3F& rRVector)
{
	mDVector = rDVector;
	mUVector = rUVector;
	mRVector = rRVector;

	Float aDet = MathF::FAbs(mDVector.Dot(mUVector.Cross(mRVector)));
	if (MathF::FAbs(1.0F - aDet) > 0.01F)
	{
		// The input vectors do not appear to form an orthonormal set. Time
		// to renormalize.
		Vector3F::Orthonormalize(mDVector, mUVector, mRVector);
	}
}

//----------------------------------------------------------------------------
void Camera::SetFrame(const Vector3F& rLocation, const Vector3F& rDVector,
	const Vector3F& rUVector, const Vector3F& rRVector)
{
	mLocation = rLocation;
	SetAxes(rDVector, rUVector, rRVector);
}

//----------------------------------------------------------------------------
void Camera::LookAt(const Vector3F& rLocation, const Vector3F& rLookAt,
	const Vector3F& rUp)
{
	const Vector3F direction = rLookAt - rLocation;
	SetFrame(rLocation, direction, rUp, direction.Cross(rUp));
}

//----------------------------------------------------------------------------
void Camera::SetViewport(Float left, Float right, Float top, Float bottom)
{
	mPortLeft = left;
	mPortRight = right;
	mPortTop = top;
	mPortBottom = bottom;
}

//----------------------------------------------------------------------------
void Camera::GetViewport(Float& rLeft, Float& rRight, Float& rTop,
	Float& rBottom)
{
	rLeft = mPortLeft;
	rRight = mPortRight;
	rTop = mPortTop;
	rBottom = mPortBottom;
}

//----------------------------------------------------------------------------
Matrix4F Camera::GetProjectionMatrix() const
{
	Float n = mFrustum[0];
	Float f = mFrustum[1];
	Float b = mFrustum[2];
	Float t = mFrustum[3];
	Float l = mFrustum[4];
	Float r = mFrustum[5];
	return Matrix4F(2 * n / (r - l),	0,					(r + l) / (r - l),		0,
		            0,					2 * n / (t - b),	(t + b) / (t - b),		0,
					0,					0,					-(f + n) / (f - n),		-(2 * n * f) / (f - n),
					0,					0,					-1,						0 );
}

//----------------------------------------------------------------------------
Matrix4F Camera::GetViewMatrix() const
{
	return Matrix4F(mRVector.X(), mUVector.X(), mDVector.X(),	0,
					mRVector.Y(), mUVector.Y(), mDVector.Y(),	0,
					mRVector.Z(), mUVector.Z(), mDVector.Z(),	0,
					0,			  0,			0,				1);
}

//----------------------------------------------------------------------------
Vector3F Camera::ScreenToWorldPoint(const Vector2F& rScreenPoint) const
{
	Vector4F worldPoint = (GetProjectionMatrix() * GetViewMatrix()).Inverse() * Vector4F(rScreenPoint.X(), rScreenPoint.Y(), 0, 1);
	return Vector3F(worldPoint.X(), worldPoint.Y(), worldPoint.Z());
}

//----------------------------------------------------------------------------
void Camera::LookAtScreenPoint(const Vector2F& rScreenPoint)
{
	LookAt(mLocation, ScreenToWorldPoint(rScreenPoint), mUVector);
}