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

	// applying accumulators
	mMove *= deltaTime;
	mAngleY += (mAngleYIncrement * deltaTime);
	mAngleX += mAngleXIncrement * deltaTime;
	mAngleX = MathF::Min(MathF::Max(mAngleX, -mMaxVerticalAngle), mMaxVerticalAngle); // clamping the value

	mspCamera->SetFrame(mspCamera->GetLocation() + mMove, GetDirection(), Vector3F::UNIT_Y, GetLeft());

	// clearing accumulators
	mMove = Vector3F::ZERO;
	mAngleXIncrement = 0;
	mAngleYIncrement = 0;

	// update last application time
	mLastAppTime = appTime;

	return true;
}

//----------------------------------------------------------------------------
Vector3F FirstPersonController::GetDirection()
{
	Matrix34F rotateX, rotateY;
	rotateY.FromAxisAngle(Vector3F::UNIT_Y, mAngleY);
	rotateX.FromAxisAngle(Vector3F::UNIT_X, mAngleX);
	return rotateY * rotateX * Vector3F::UNIT_Z;
}

//----------------------------------------------------------------------------
Vector3F FirstPersonController::GetBackward()
{
	Matrix34F rotateY;
	rotateY.FromAxisAngle(Vector3F::UNIT_Y, mAngleY);
	return rotateY * Vector3F::UNIT_Z;
}

//----------------------------------------------------------------------------
Vector3F FirstPersonController::GetLeft()
{
	Matrix34F rotateY;
	rotateY.FromAxisAngle(Vector3F::UNIT_Y, -(MathF::PI / 2));
	return rotateY * GetBackward();
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
	mMove -= GetBackward() * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::MoveBackward()
{
	mMove += GetBackward() * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::StrafeLeft()
{
	mMove += GetLeft() * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::StrafeRight()
{
	mMove -= GetLeft() * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::LookUp(const Vector2F& rLookUp)
{
	if (rLookUp.X() > mLookUpDeadZone.X())
	{
		mAngleYIncrement += mRotateSpeed * (rLookUp.X() / mLookUpDeadZone.X());
	}

	else if (rLookUp.X() < -mLookUpDeadZone.X())
	{
		mAngleYIncrement += mRotateSpeed * (rLookUp.X() / mLookUpDeadZone.X());
	}

	if (rLookUp.Y() > mLookUpDeadZone.Y())
	{
		mAngleXIncrement += mRotateSpeed * (rLookUp.Y() / mLookUpDeadZone.Y());
	}

	else if (rLookUp.Y() < -mLookUpDeadZone.Y())
	{
		mAngleXIncrement += mRotateSpeed * (rLookUp.Y() / mLookUpDeadZone.Y());
	}
}