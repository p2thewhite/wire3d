#include "Player.h"

#include "ProbeRobot.h"
#include "BulletUtils.h"
#include "WireGeometry.h"
#include "WireStandardMesh.h"
#include "WireStateWireframe.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Player, Controller);

//----------------------------------------------------------------------------
Player::Player(Camera* pCamera)
	:
	mTotalHealth(100.0F),
	mHealth(100.0F),
	mMaximumShootingDistance(100.0F),
	mMaximumVerticalAngle(MathF::PI / 4),
	mMoveSpeed(2.5F),
	mRotateSpeed(MathF::PI / 9),
	mCharacterWidth(1),
	mCharacterHeight(1),
	mPitch(0),
	mYaw(0),
	mPitchIncrement(0),
	mYawIncrement(0),
	mMove(Vector3F::ZERO),
	mLookAt(Vector2F::ZERO),
	mpNode(NULL),
	mJump(false),
	mShoot(false)
{
	WIRE_ASSERT(pCamera);
	mspCamera = pCamera;
}

//----------------------------------------------------------------------------
Bool Player::Update(Double appTime)
{
	Float deltaTime = GetDeltaTime(appTime);

	InitializeIfNecessary();

	// Apply accumulators
	mMove *= deltaTime;
	mYaw += mYawIncrement * deltaTime;

	if (mYaw > MathF::TWO_PI) 
	{
		mYaw = mYaw - MathF::TWO_PI;
	}
	else if (mYaw < -MathF::TWO_PI)
	{
		mYaw = MathF::TWO_PI - mYaw;
	}

	mPitch += (mPitchIncrement * deltaTime);
	mPitch = MathF::Clamp(-mMaximumVerticalAngle, mPitch, mMaximumVerticalAngle);

	// Calculate rotation matrices
	mRotationX.FromAxisAngle(Vector3F::UNIT_X, mPitch);
	mRotationY.FromAxisAngle(Vector3F::UNIT_Y, mYaw);

	// Calculate up, gunDirection and lookup
	mEyeDirection = mRotationY * mRotationX * Vector3F::UNIT_Z;
	mUp = mRotationY * mRotationX * Vector3F::UNIT_Y;
	mForward = mRotationY * Vector3F::UNIT_Z;

	Matrix34F rotation;
	rotation.FromAxisAngle(Vector3F::UNIT_Y, -(MathF::PI / 2));
	mRight = rotation * mForward;

	DoShooting();
	UpdatePlayerNode();
	UpdateGunRotation();
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
	if (mpNode)
	{
		return;
	}

	mpNode = DynamicCast<Node>(GetObject());
	WIRE_ASSERT(mpNode);

	mpGun = mpNode->GetChildByName("Gun");
	if (mpGun)
	{
		mGunStartingRotation = mpGun->World.GetRotate();
	}

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
	mMaximumVerticalAngle = MathF::DEG_TO_RAD * (maximumVerticalAngle);
}

//----------------------------------------------------------------------------
void Player::SetMaximumShootingDistance(Float maximumShootingDistance)
{
	mMaximumShootingDistance = maximumShootingDistance;
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
	mLookAt = rLookAt;

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
void Player::UpdatePlayerNode()
{
	mpNode->Local.SetTranslate(GetPosition());
	mpNode->Local.SetRotate(mRotationY * mRotationX);
}

//----------------------------------------------------------------------------
void Player::UpdateGunRotation()
{
	if (mpGun == NULL)
	{
		return;
	}

	Vector3F lookAtWorldSpace = mspCamera->ScreenToWorldPoint(mLookAt);
	// Increase the z to a slighter gunRotation
	lookAtWorldSpace.Z() = 400;

	Vector3F gunDirection = mpGun->Local.GetRotate() * Vector3F::UNIT_Z;

	// Calculate look gunRotation from look coordinates in world space
	Vector3F axis = gunDirection.Cross(lookAtWorldSpace);
	Float angle = gunDirection.GetAngle(lookAtWorldSpace);

	Matrix3F gunRotation;
	gunRotation.FromAxisAngle(axis, angle);
	mpGun->Local.SetRotate(mpGun->Local.GetRotate() * gunRotation);
}

//----------------------------------------------------------------------------
void Player::DoShooting()
{
	// Remove previous ray
	Spatial* pRay = mpNode->GetChildByName("Ray");

	if (pRay) 
	{
		mpNode->DetachChild(pRay);
	}

	// If not shooting, exit
	if (!mShoot) 
	{
		return;
	}

	btVector3 rayStart = BulletUtils::Convert(GetPosition());
	btVector3 rayEnd = rayStart + BulletUtils::Convert(mEyeDirection * mMaximumShootingDistance);

	btCollisionWorld::ClosestRayResultCallback hitCallback(rayStart, rayEnd);

	mpPhysicsWorld->rayTest(rayStart, rayEnd, hitCallback);
	if (hitCallback.hasHit()) 
	{
		Vector3F hitPoint = BulletUtils::Convert(hitCallback.m_hitPointWorld);
		CreateRay(GetPosition().Distance(hitPoint));

		ProbeRobot* pProbeRobotController = static_cast<ProbeRobot*>(hitCallback.m_collisionObject->getUserPointer());

		if (pProbeRobotController) 
		{
			pProbeRobotController->TakeDamage(5.0F);
		}
	}

	mShoot = false;
}

//----------------------------------------------------------------------------
void Player::CreateRay(Float size)
{
	Geometry* pRay = StandardMesh::CreateCylinder(5, 5, 0.1F, size, 0, 4,
		false);
	pRay->SetName("Ray");
	mpNode->AttachChild(pRay);
	mpNode->UpdateRS();

	Vector3F gunEnd = mpGun->Local.GetTranslate() + (mpGun->Local.GetRotate()
		* Vector3F(0, 0, 1.2F)) * (size * 0.5F);
	pRay->Local.SetTranslate(gunEnd);
	pRay->Local.SetRotate(mpGun->Local.GetRotate());
}

//----------------------------------------------------------------------------
void Player::UpdateCamera()
{
	mspCamera->SetFrame(GetPosition(), mEyeDirection, mUp, mRight);
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

//----------------------------------------------------------------------------
Float Player::GetDeltaTime(Double appTime)
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

	return deltaTime;
}
