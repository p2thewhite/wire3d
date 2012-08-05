#include "FirstPersonController.h"
#include "BulletUtils.h"

using namespace Wire;

//----------------------------------------------------------------------------
FirstPersonController::FirstPersonController(const Vector3F& rPosition, Camera* pCamera)
	:
	mMaxPitch(MathF::PI / 4),
	mMoveSpeed(2.5f),
	mRotateSpeed(MathF::PI / 9),
	mCharacterWidth(1),
	mCharacterHeight(1),
	mPitch(0),
	mYaw(0),
	mPitchIncrement(0),
	mYawIncrement(0),
	mMove(Vector3F::ZERO),
	mJump(false)
{
	if (!pCamera)
	{
		return;
	}

	mspCamera = pCamera;
	mspCamera->SetFrame(rPosition, Vector3F(0, 0, -1), Vector3F::UNIT_Y, Vector3F::UNIT_X);
}

//----------------------------------------------------------------------------
Bool FirstPersonController::Update(Double appTime)
{
	static Double lastApplicationTime = 0;
	Float deltaTime = static_cast<Float>(lastApplicationTime - appTime);
	lastApplicationTime = appTime;

	// apply accumulators
	mMove *= deltaTime;
	mYaw += (mYawIncrement * deltaTime);
	mPitch += (mPitchIncrement * deltaTime);
	mPitch = MathF::Clamp(-mMaxPitch, mPitch, mMaxPitch);

	// calculate rotation matrices
	Matrix34F rotationX, rotationY;
	rotationX.FromAxisAngle(Vector3F::UNIT_X, mPitch);
	rotationY.FromAxisAngle(Vector3F::UNIT_Y, mYaw);

	mLookAt = rotationY * rotationX * Vector3F::UNIT_Z;
	mUp = rotationY * rotationX * Vector3F::UNIT_Y;
	mBack = rotationY * Vector3F::UNIT_Z;

	rotationY.FromAxisAngle(Vector3F::UNIT_Y, -(MathF::PI / 2));
	mLeft = rotationY * mBack;

	UpdateCamera();
	MoveCharacterController();

	// reset accumulators
	mMove = Vector3F::ZERO;
	mPitchIncrement = 0;
	mYawIncrement = 0;
	mJump = false;

	return true;
}

//----------------------------------------------------------------------------
void FirstPersonController::Register(btDynamicsWorld* pPhysicsWorld)
{
	WIRE_ASSERT(pPhysicsWorld);

	// set physics world reference
	mpPhysicsWorld = pPhysicsWorld;

	btTransform transform;
	transform.setIdentity();

	// set position
	transform.setOrigin(BulletUtils::Convert(mspCamera->GetLocation()));

	mpGhostObject = WIRE_NEW btPairCachingGhostObject();
	mpGhostObject->setWorldTransform(transform);

	pPhysicsWorld->getPairCache()->setInternalGhostPairCallback(WIRE_NEW btGhostPairCallback());

	btConvexShape* capsuleShape = WIRE_NEW btCapsuleShape(mCharacterWidth, mCharacterHeight);
	mpGhostObject->setCollisionShape(capsuleShape);
	mpGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	mpPhysicsCharacterController = WIRE_NEW btKinematicCharacterController(
		mpGhostObject, capsuleShape, mStepHeight);

	// collide only with static objects (for now)
	mpPhysicsWorld->addCollisionObject(mpGhostObject, btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	mpPhysicsWorld->addAction(mpPhysicsCharacterController);
}

//----------------------------------------------------------------------------
void FirstPersonController::SetHeadHeight(Float headHeight)
{
	mHeadHeight = headHeight;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetCharacterWidth(Float characterWidth)
{
	mCharacterWidth = characterWidth;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetCharacterHeight(Float characterHeight)
{
	mCharacterHeight = characterHeight;
}

//----------------------------------------------------------------------------
void FirstPersonController::SetStepHeight(Float stepHeight)
{
	mStepHeight = stepHeight;
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
void FirstPersonController::SetMaximumVerticalAngle(Float maximumVerticalAngle)
{
	mMaxPitch = MathF::DEG_TO_RAD * (maximumVerticalAngle);
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
void FirstPersonController::Jump()
{
	mJump = true;
}

//----------------------------------------------------------------------------
void FirstPersonController::LookAt(const Vector2F& rLookAt)
{
	if (rLookAt.X() > mLookUpDeadZone.X())
	{
		mYawIncrement += mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}
	else if (rLookAt.X() < -mLookUpDeadZone.X())
	{
		mYawIncrement += mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}

	if (rLookAt.Y() > mLookUpDeadZone.Y())
	{
		mPitchIncrement += mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
	else if (rLookAt.Y() < -mLookUpDeadZone.Y())
	{
		mPitchIncrement += mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
}

//----------------------------------------------------------------------------
void FirstPersonController::UpdateCamera()
{
	btVector3 origin = mpGhostObject->getWorldTransform().getOrigin();
	mspCamera->SetFrame(Vector3F(origin.x(), origin.y() + mHeadHeight, origin.z()), mLookAt, mUp, mLeft);
}

//----------------------------------------------------------------------------
void FirstPersonController::MoveCharacterController()
{
	if (mJump)
	{
		mpPhysicsCharacterController->jump();
	}

	mpPhysicsCharacterController->setWalkDirection(BulletUtils::Convert(mMove));
}
