#include "ProbeRobot.h"

#include "BulletUtils.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(ProbeRobot, Controller);

//----------------------------------------------------------------------------
ProbeRobot::ProbeRobot(Spatial* pPlayerSpatial)
	:
	mTotalHealth(100.0F),
	mHealth(100.0F),
	mSpeed(7.5F),
	mMaximumPlayerDistanceSquared(100.0F)
{
	WIRE_ASSERT(pPlayerSpatial);
	mspPlayerSpatial = pPlayerSpatial;
}

//----------------------------------------------------------------------------
Bool ProbeRobot::Update(Double appTime)
{
	Float deltaTime = GetDeltaTime(appTime);

	if (mHealth <= 0)
	{
		Die();
		return true;
	}

	CalculateMovementAndRotation(deltaTime);
	UpdateModel();
	MovePhysicsEntity();

	mMove = Vector3F::ZERO;

	return true;
}

//----------------------------------------------------------------------------
void ProbeRobot::Die()
{
	// DEBUG:
	//System::Print("Probe robot has died\n");

	Spatial* pSpatial = DynamicCast<Spatial>(GetSceneObject());
	WIRE_ASSERT(pSpatial);
	pSpatial->DetachController(this);
	mpGhostObject->setUserPointer(NULL);
}

//----------------------------------------------------------------------------
void ProbeRobot::Register(btDynamicsWorld* pPhysicsWorld)
{
	WIRE_ASSERT(pPhysicsWorld);
	if (!pPhysicsWorld)
	{
		return;
	}

	mpGhostObject = WIRE_NEW btPairCachingGhostObject();

	btConvexShape* pConvexShape = WIRE_NEW btCapsuleShape(2.0F, 3.0F);
	mpGhostObject->setCollisionShape(pConvexShape);
	mpGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// Add a reference to the controller in the physics object
	mpGhostObject->setUserPointer(this);

	// Create physics entity
	mpPhysicsEntity = WIRE_NEW btKinematicCharacterController(mpGhostObject,
		pConvexShape, 0.35f);

	// Collide only with static objects (for now)
	pPhysicsWorld->addCollisionObject(mpGhostObject,
		btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter |
		btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter);
	pPhysicsWorld->addAction(mpPhysicsEntity);

	Spatial* pSpatial = DynamicCast<Spatial>(GetSceneObject());
	WIRE_ASSERT(pSpatial);
	// Set physics entity position
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(BulletUtils::Convert(pSpatial->Local.GetTranslate()));
	mpGhostObject->setWorldTransform(transform);
}

//----------------------------------------------------------------------------
void ProbeRobot::SetTotalHealth(Float totalHealth)
{
	mTotalHealth = totalHealth;
	mHealth = mTotalHealth;
}

//----------------------------------------------------------------------------
Float ProbeRobot::GetTotalHealth()
{
	return mTotalHealth;
}

//----------------------------------------------------------------------------
Float ProbeRobot::GetHealth()
{
	return mHealth;
}

//----------------------------------------------------------------------------
Vector3F ProbeRobot::GetPosition()
{
	btVector3 origin = mpGhostObject->getWorldTransform().getOrigin();
	return Vector3F(origin.x(), origin.y(), origin.z());
}

//----------------------------------------------------------------------------
void ProbeRobot::UpdateModel()
{
	Spatial* pSpatial = DynamicCast<Spatial>(GetSceneObject());
	WIRE_ASSERT(pSpatial);
	pSpatial->Local.SetTranslate(GetPosition());
	pSpatial->Local.SetRotate(mRotation);
}

//----------------------------------------------------------------------------
void ProbeRobot::MovePhysicsEntity()
{
	mpPhysicsEntity->setWalkDirection(BulletUtils::Convert(mMove));
}

//----------------------------------------------------------------------------
void ProbeRobot::TakeDamage(Float damage)
{
	// DEBUG:
	//System::Print("Probe robot has taken %.1f point of damage\n", damage);

	mHealth -= damage;
}

//----------------------------------------------------------------------------
void ProbeRobot::CalculateMovementAndRotation(Float deltaTime)
{
	Vector3F playerPosition = mspPlayerSpatial->Local.GetTranslate();
	Vector3F probeRobotPosition = GetPosition();
	Float squaredDistance = MathF::FAbs(playerPosition.SquaredDistance(
		probeRobotPosition));

	Vector3F direction = playerPosition - probeRobotPosition;
	direction.Normalize();

	Float angle = direction.Angle(Vector3F::UNIT_Z);

	// Correcting angle sign
	if (playerPosition.Cross(probeRobotPosition).Z() < 0)
	{
		angle = -angle;
	}

	mRotation.FromAxisAngle(Vector3F::UNIT_Y, angle);

	if (squaredDistance > mMaximumPlayerDistanceSquared)
	{
		mMove = direction * mSpeed * deltaTime;
	}
}

//----------------------------------------------------------------------------
Float ProbeRobot::GetDeltaTime(Double appTime)
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
