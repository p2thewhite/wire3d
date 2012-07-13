#include "FirstPersonController.h"

using namespace Wire;

//----------------------------------------------------------------------------
FirstPersonController::FirstPersonController(const Vector3F& rPosition, Camera* pCamera)
	:
	mLastAppTime(0),
	mAngleX(0),
	mAngleY(0),
	mAngleXIncrement(0),
	mAngleYIncrement(0),
	mMove(Vector3F::ZERO),
	mMaxVerticalAngle(MathF::PI / 4),
	mMoveSpeed(2.5f),
	mRotateSpeed(MathF::PI / 9)
{
	if (!pCamera)
	{
		return;
	}

	// ---
	mspCamera = pCamera;
	mspCamera->SetFrame(rPosition, Vector3F(0, 0, 1), Vector3F::UNIT_Y, Vector3F::UNIT_X);
}

//----------------------------------------------------------------------------
Bool FirstPersonController::Update(Double appTime)
{
	Float deltaTime = static_cast<Float>(mLastAppTime - appTime);

	// apply accumulators
	mMove *= deltaTime;
	mAngleY += (mAngleYIncrement * deltaTime);
	mAngleX += mAngleXIncrement * deltaTime;
	mAngleX = MathF::Min(MathF::Max(mAngleX, -mMaxVerticalAngle), mMaxVerticalAngle); // clamping the value

	// calculate rotation matrices
	Matrix34F rotationX, rotationY;
	rotationX.FromAxisAngle(Vector3F::UNIT_X, mAngleX);
	rotationY.FromAxisAngle(Vector3F::UNIT_Y, mAngleY);

	mLookAt = rotationY * rotationX * Vector3F::UNIT_Z;
	mUp = rotationY * rotationX * Vector3F::UNIT_Y;
	mBack = rotationY * Vector3F::UNIT_Z;

	rotationY.FromAxisAngle(Vector3F::UNIT_Y, -(MathF::PI / 2));
	mLeft = rotationY * mBack;

	// actually rotate the camera
	mspCamera->SetFrame(mspCamera->GetLocation() + mMove, mLookAt, mUp, mLeft);

	// reset accumulators
	mMove = Vector3F::ZERO;
	mAngleXIncrement = 0;
	mAngleYIncrement = 0;

	// update last application time
	mLastAppTime = appTime;

	return true;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetMoveSpeed(Float moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetRotateSpeed(Float rotateSpeed)
{
	mRotateSpeed = rotateSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetLookUpDeadZone(const Vector2F& rLookUpDeadZone)
{
	mLookUpDeadZone = rLookUpDeadZone;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetMaxVerticalAngle(Float maxVerticalAngle)
{
	mMaxVerticalAngle = maxVerticalAngle;
}

//----------------------------------------------------------------------------
void FirstPersonController::MoveForward()
{
	mMove -= mBack * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::MoveBackward()
{
	mMove += mBack * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::StrafeLeft()
{
	mMove += mLeft * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::StrafeRight()
{
	mMove -= mLeft * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::LookAt(const Vector2F& rLookAt)
{
	if (rLookAt.X() > mLookUpDeadZone.X())
	{
		mAngleYIncrement += mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}

	else if (rLookAt.X() < -mLookUpDeadZone.X())
	{
		mAngleYIncrement += mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}

	if (rLookAt.Y() > mLookUpDeadZone.Y())
	{
		mAngleXIncrement += mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}

	else if (rLookAt.Y() < -mLookUpDeadZone.Y())
	{
		mAngleXIncrement += mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
}