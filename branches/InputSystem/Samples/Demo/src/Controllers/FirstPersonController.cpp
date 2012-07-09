#include "FirstPersonController.h"

using namespace Wire;

//----------------------------------------------------------------------------
FirstPersonController::FirstPersonController(const Vector3F& rPosition, Camera* pCamera) :	mLastAppTime(0),
	mMove(Vector3F::ZERO), mMoveSpeed(2.5f), mRotateSpeed(MathF::PI / 9), mMaxVerticalAngle(MathF::PI / 4), mAngleX(0), mAngleY(0), mAngleXIncrement(0), mAngleYIncrement(0)
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
	// calculate the elapsed time
	Float deltaTime = static_cast<Float>(mLastAppTime - appTime);

	mMove *= deltaTime;
	mAngleX += mAngleXIncrement * deltaTime;
	mAngleX = MathF::Min(MathF::Max(mAngleX, -mMaxVerticalAngle), mMaxVerticalAngle);

	mAngleY += (mAngleYIncrement * deltaTime);

	Vector3F position = mspCamera->GetLocation() + mMove;

	mspCamera->SetFrame(position, GetDirection(), Vector3F::UNIT_Y, GetLeft());

	// clearing accumulator
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
Vector3F FirstPersonController::GetForward()
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
	return rotateY * GetForward();
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
	mMove -= GetForward() * mMoveSpeed;
}

//----------------------------------------------------------------------------
void FirstPersonController::MoveBackward()
{
	mMove += GetForward() * mMoveSpeed;
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
		mAngleXIncrement -= mRotateSpeed * (rLookUp.Y() / mLookUpDeadZone.Y());
	}

	else if (rLookUp.Y() < -mLookUpDeadZone.Y())
	{
		mAngleXIncrement -= mRotateSpeed * (rLookUp.Y() / mLookUpDeadZone.Y());
	}
}