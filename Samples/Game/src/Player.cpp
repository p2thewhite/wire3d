#include "Player.h"

#include "ProbeRobot.h"
#include "BulletUtils.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Player, Controller);

//----------------------------------------------------------------------------
Player::Player(Camera* pCamera)
	:
	mpNode(NULL),
	mTotalHealth(100.0f),
	mHealth(100.0f),
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
	WIRE_ASSERT(pCamera);
	mspCamera = pCamera;
}

//----------------------------------------------------------------------------
Bool Player::Update(Double appTime)
{
	static Double lastApplicationTime = 0;
	Float deltaTime;

	if (lastApplicationTime == 0) 
	{
		deltaTime = 0;
	} 
	else 
	{
		deltaTime = (Float)(appTime - lastApplicationTime);
	}
	lastApplicationTime = appTime;

	InitializeIfNecessary();

	// Apply accumulators
	mMove *= deltaTime;
	mYaw += (mYawIncrement * deltaTime);
	mPitch += (mPitchIncrement * deltaTime);
	mPitch = MathF::Clamp(-mMaxPitch, mPitch, mMaxPitch);

	// Calculate rotation matrices
	mRotationX.FromAxisAngle(Vector3F::UNIT_X, mPitch);
	mRotationY.FromAxisAngle(Vector3F::UNIT_Y, mYaw);

	// Calculate up, forward and lookup
	mLookAt = mRotationY * mRotationX * Vector3F::UNIT_Z;
	mUp = mRotationY * mRotationX * Vector3F::UNIT_Y;
	mForward = mRotationY * Vector3F::UNIT_Z;

	Matrix34F rotation;
	rotation.FromAxisAngle(Vector3F::UNIT_Y, -(MathF::PI / 2));
	mRight = rotation * mForward;

	UpdatePlayer();
	UpdateGun();
	DoShooting();
	UpdateCamera();
	MovePhysicsEntity();

	// Reset accumulators
	mMove = Vector3F::ZERO;
	mPitchIncrement = 0;
	mYawIncrement = 0;
	mJump = false;

	return true;
}

//----------------------------------------------------------------------------
void Player::InitializeIfNecessary()
{
	if (mpNode != NULL)
	{
		return;
	}

	mpNode = DynamicCast<Node>(GetObject());
	mpGun = mpNode->GetChildByName("Gun");
	mStartingGunRotation = mpGun->World.GetRotate();

	// Set camera position
	Vector3F cameraPosition = mpNode->World.GetTranslate();
	mspCamera->SetFrame(cameraPosition, Vector3F(0, 0, -1), Vector3F::UNIT_Y, Vector3F::UNIT_X);

	// Set physics entity position
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(BulletUtils::Convert(mspCamera->GetLocation()));
	mpGhostObject->setWorldTransform(transform);
}

//----------------------------------------------------------------------------
void Player::Register(btDynamicsWorld* pPhysicsWorld)
{
	WIRE_ASSERT(pPhysicsWorld);
	mpPhysicsWorld = pPhysicsWorld;

	mpGhostObject = WIRE_NEW btPairCachingGhostObject();

	btConvexShape* pConvexShape = WIRE_NEW btCapsuleShape(mCharacterWidth, mCharacterHeight);
	mpGhostObject->setCollisionShape(pConvexShape);
	mpGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// Add a reference to the controller in the physics object
	mpGhostObject->setUserPointer(this);

	// Create physics entity
	mpPhysicsEntity = WIRE_NEW btKinematicCharacterController(mpGhostObject, pConvexShape, mStepHeight);

	mpPhysicsWorld->addCollisionObject(mpGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | 
		btBroadphaseProxy::CharacterFilter | 
		btBroadphaseProxy::DefaultFilter);
	mpPhysicsWorld->addAction(mpPhysicsEntity);
}

//----------------------------------------------------------------------------
void Player::SetTotalHealth(Float totalHealth)
{
	mTotalHealth = totalHealth;
	mHealth = mTotalHealth;
}

//----------------------------------------------------------------------------
Float Player::GetHealth()
{
	return mHealth;
}

//----------------------------------------------------------------------------
Float Player::GetTotalHealth()
{
	return mTotalHealth;
}

//----------------------------------------------------------------------------
void Player::SetHeadHeight(Float headHeight)
{
	mHeadHeight = headHeight;
}

//----------------------------------------------------------------------------
void Player::SetCharacterWidth(Float characterWidth)
{
	mCharacterWidth = characterWidth;
}

//----------------------------------------------------------------------------
void Player::SetCharacterHeight(Float characterHeight)
{
	mCharacterHeight = characterHeight;
}

//----------------------------------------------------------------------------
void Player::SetStepHeight(Float stepHeight)
{
	mStepHeight = stepHeight;
}

//----------------------------------------------------------------------------
void Player::SetMoveSpeed(Float moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

//----------------------------------------------------------------------------
void Player::SetRotateSpeed(Float rotateSpeed)
{
	mRotateSpeed = rotateSpeed;
}

//----------------------------------------------------------------------------
void Player::SetLookUpDeadZone(const Vector2F& rLookUpDeadZone)
{
	mLookUpDeadZone = rLookUpDeadZone;
}

//----------------------------------------------------------------------------
void Player::SetMaximumVerticalAngle(Float maximumVerticalAngle)
{
	mMaxPitch = MathF::DEG_TO_RAD * (maximumVerticalAngle);
}

//----------------------------------------------------------------------------
void Player::MoveForward()
{
	mMove += mForward * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::MoveBackward()
{
	mMove -= mForward * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::StrafeLeft()
{
	mMove -= mRight * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::StrafeRight()
{
	mMove += mRight * mMoveSpeed;
}

//----------------------------------------------------------------------------
void Player::Jump()
{
	mJump = true;
}

//----------------------------------------------------------------------------
void Player::Shoot()
{
	mShoot = true;
}

//----------------------------------------------------------------------------
void Player::LookAt(const Vector2F& rLookAt)
{
	if (rLookAt.X() > mLookUpDeadZone.X())
	{
		mYawIncrement -= mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}
	else if (rLookAt.X() < -mLookUpDeadZone.X())
	{
		mYawIncrement -= mRotateSpeed * (rLookAt.X() / mLookUpDeadZone.X());
	}

	if (rLookAt.Y() > mLookUpDeadZone.Y())
	{
		mPitchIncrement -= mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
	else if (rLookAt.Y() < -mLookUpDeadZone.Y())
	{
		mPitchIncrement -= mRotateSpeed * (rLookAt.Y() / mLookUpDeadZone.Y());
	}
}

//----------------------------------------------------------------------------
Vector3F Player::GetPosition()
{
	btVector3 origin = mpGhostObject->getWorldTransform().getOrigin();
	return Vector3F(origin.x(), origin.y() + mHeadHeight, origin.z());
}

//----------------------------------------------------------------------------
void Player::UpdatePlayer()
{
	mpNode->Local.SetTranslate(GetPosition());
	mpNode->Local.SetRotate(mRotationY * mRotationX);
}

//----------------------------------------------------------------------------
void Player::UpdateGun()
{
	if (mpGun == NULL)
	{
		return;
	}

	// Clamping between -0.5 and 0.5 radians (approx. -30 and 30 degrees)
	Float weaponPitch = MathF::Clamp(-0.5, mPitch * 0.1f, 0.5);
	Float weaponYaw = MathF::Clamp(-0.5, mYaw * 0.1f, 0.5);

	// Calculate scaled rotations
	Matrix3F rotationX;
	rotationX.FromAxisAngle(Vector3F::UNIT_X, weaponPitch);
	Matrix3F rotationY;
	// FIXME: gun model starts at 90 X rotation, so the Y corresponds to the Z
	rotationY.FromAxisAngle(Vector3F::UNIT_Z, weaponYaw);

	mpGun->Local.SetRotate(mStartingGunRotation * rotationX * rotationY);
}

//----------------------------------------------------------------------------
void Player::DoShooting()
{
	if (!mShoot) 
	{
		return;
	}

	btVector3 rayStart = BulletUtils::Convert(GetPosition());
	// FIXME: Add max shoot reach field
	btVector3 rayEnd = rayStart + BulletUtils::Convert(mLookAt * 50.0f);

	btCollisionWorld::ClosestRayResultCallback hitCallback(rayStart, rayEnd);
	hitCallback.m_collisionFilterMask = btBroadphaseProxy::CharacterFilter;

	mpPhysicsWorld->rayTest(rayStart, rayEnd, hitCallback);
	if (hitCallback.hasHit()) 
	{
		ProbeRobot* pProbeRobotController = static_cast<ProbeRobot*>(hitCallback.m_collisionObject->getUserPointer());

		if (pProbeRobotController) 
		{
			pProbeRobotController->TakeDamage(1.0f);
		}
	}

	mShoot = false;
}

//----------------------------------------------------------------------------
void Player::UpdateCamera()
{
	mspCamera->SetFrame(GetPosition(), mLookAt, mUp, mRight);
}

//----------------------------------------------------------------------------
void Player::MovePhysicsEntity()
{
	if (mJump)
	{
		mpPhysicsEntity->jump();
	}

	mpPhysicsEntity->setWalkDirection(BulletUtils::Convert(mMove));
}

//----------------------------------------------------------------------------
void Player::TakeDamage(Float damage)
{
	mHealth -= damage;
}