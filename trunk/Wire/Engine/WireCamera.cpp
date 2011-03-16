#include "WireCamera.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Camera, Object);

//----------------------------------------------------------------------------
Camera::Camera()
{
	SetFrustum(-0.5F, 0.5F, -0.4F, 0.4F, 1.0F, 100.0F);
	SetViewport(0.0F, 1.0F, 1.0F, 0.0F);
	SetFrame(Vector3F::ZERO, -Vector3F::UNIT_Z, Vector3F::UNIT_Y,
		Vector3F::UNIT_X);
}

//----------------------------------------------------------------------------
Camera::Camera(const Vector3F& location, const Vector3F& direction,
	const Vector3F& up)
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
