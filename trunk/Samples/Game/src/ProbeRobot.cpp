#include "ProbeRobot.h"

#include "BulletUtils.h"

using namespace Wire;

//----------------------------------------------------------------------------
ProbeRobot::ProbeRobot(Spatial* pPlayerSpatial)
	:
	mpSpatial(NULL),
	mTotalHealth(100.0f),
	mHealth(100.0f),
	mSpeed(5.0f),
	mSquaredMaximumPlayerDistance(25.0f)
{
	WIRE_ASSERT(pPlayerSpatial);
	mspPlayerSpatial = pPlayerSpatial;
}

//----------------------------------------------------------------------------
Bool ProbeRobot::Update(Double appTime)
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

	if (mHealth <= 0)
	{
		Die();
		return true;
	}

	Vector3F playerPosition = mspPlayerSpatial->Local.GetTranslate();
	Vector3F probeRobotPosition = GetPosition();
	Float squaredDistance = MathF::FAbs(playerPosition.SquaredDistance(probeRobotPosition));

	Vector3F direction = playerPosition - probeRobotPosition;
	direction.Normalize();

	Float angle = direction.GetAngle(Vector3F::UNIT_Z);

	// Correcting angle sign
	if (playerPosition.Cross(probeRobotPosition).Z() < 0)
	{
		angle = -angle;
	}

	// FIXME: probe robot model starts at -90 X rotation, so the Y corresponds to the Z
	mRotation.FromAxisAngle(Vector3F::UNIT_Z, angle);

	if (squaredDistance > mSquaredMaximumPlayerDistance)
	{
		mMove = direction * mSpeed * deltaTime;
	}

	UpdateSpatial();
	MovePhysicsEntity();

	mMove = Vector3F::ZERO;

	return true;
}

//----------------------------------------------------------------------------
void ProbeRobot::InitializeIfNecessary()
{
	if (mpSpatial != NULL) 
	{
		return;
	}

	mpSpatial = DynamicCast<Spatial>(GetObject());
	mStartingRotation = mpSpatial->World.GetRotate();

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(BulletUtils::Convert(mpSpatial->Local.GetTranslate()));
	mpGhostObject->setWorldTransform(transform);
}

//----------------------------------------------------------------------------
void ProbeRobot::Die()
{
	// DEBUG:
	System::Print("Probe robot has died\n");

	mpSpatial->DetachController(this);
	mpGhostObject->setUserPointer(NULL);
}

//----------------------------------------------------------------------------
void ProbeRobot::Register(btDynamicsWorld* pPhysicsWorld)
{
	WIRE_ASSERT(pPhysicsWorld);
	mpPhysicsWorld = pPhysicsWorld;

	mpGhostObject = WIRE_NEW btPairCachingGhostObject();

	btConvexShape* pConvexShape = WIRE_NEW btCapsuleShape(2.0f, 3.0f);
	mpGhostObject->setCollisionShape(pConvexShape);
	mpGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	// Add a reference to the controller in the physics object
	mpGhostObject->setUserPointer(this);

	// Create physics entity
	mpPhysicsEntity = WIRE_NEW btKinematicCharacterController(mpGhostObject, pConvexShape, 0.35f);

	// Collide only with static objects (for now)
	mpPhysicsWorld->addCollisionObject(mpGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter);
	mpPhysicsWorld->addAction(mpPhysicsEntity);
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
void ProbeRobot::SetSpeed(Float speed)
{
	mSpeed = speed;
}

//----------------------------------------------------------------------------
void ProbeRobot::SetMaximumPlayerDistance(Float maximumPlayerDistance)
{
	mSquaredMaximumPlayerDistance = maximumPlayerDistance * maximumPlayerDistance;
}

//----------------------------------------------------------------------------
Vector3F ProbeRobot::GetPosition()
{
	btVector3 origin = mpGhostObject->getWorldTransform().getOrigin();
	return Vector3F(origin.x(), origin.y(), origin.z());
}

//----------------------------------------------------------------------------
void ProbeRobot::UpdateSpatial()
{
	mpSpatial->Local.SetTranslate(GetPosition());
	mpSpatial->Local.SetRotate(mStartingRotation * mRotation);
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
	System::Print("Probe robot has taken %.1f point of damage\n", damage);

	mHealth -= damage;
}
