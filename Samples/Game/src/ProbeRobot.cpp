#include "ProbeRobot.h"

#include "PhysicsWorld.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(ProbeRobot, Controller);

//----------------------------------------------------------------------------
ProbeRobot::ProbeRobot(Spatial* pPlayerSpatial, Spatial* pHealthBar)
	:
	mspPlayerSpatial(pPlayerSpatial),
	mspHealthBar(pHealthBar),
	mTotalHealth(100.0F),
	mHealth(100.0F),
	mSpeed(7.5F),
	mMaximumPlayerDistanceSquared(100.0F)
{
	WIRE_ASSERT(mspPlayerSpatial);
	WIRE_ASSERT(mspHealthBar);

	mHealthBarScale = mspHealthBar->Local.GetScale();
}

//----------------------------------------------------------------------------
Bool ProbeRobot::Update(Double appTime)
{
	Float elapsedTime = static_cast<Float>(appTime - mLastApplicationTime);
	if (mLastApplicationTime == -MathD::MAX_REAL)
	{
		elapsedTime = 0.0F;
	}

	mLastApplicationTime = appTime;

	if (mHealth <= 0)
	{
		Die();
		return true;
	}

	CalculateMovementAndRotation(elapsedTime);

	// update the red health bar
	Float healthRatio = mHealth / mTotalHealth;
	Vector3F healthBarScale = mHealthBarScale;
	healthBarScale.X() = MathF::Max(healthBarScale.X() * healthRatio, 0.0001F);
	mspHealthBar->Local.SetScale(healthBarScale);

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
	transform.setOrigin(PhysicsWorld::Convert(pSpatial->Local.GetTranslate()));
	mpGhostObject->setWorldTransform(transform);
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
	btVector3 origin = mpGhostObject->getWorldTransform().getOrigin();
	const Vector3F probeRobotPosition(origin.x(), origin.y(), origin.z());

	Vector3F playerPosition = mspPlayerSpatial->Local.GetTranslate();
	Float squaredDistance = playerPosition.SquaredDistance(probeRobotPosition);

	Vector3F direction = playerPosition - probeRobotPosition;
	direction.Normalize();

	Float angle = direction.Angle(Vector3F::UNIT_Z);

	// Correcting angle sign
	if (playerPosition.Cross(probeRobotPosition).Z() < 0)
	{
		angle = -angle;
	}

	Matrix3F rotation(Vector3F::UNIT_Y, angle);
	Vector3F move = Vector3F::ZERO;

	if (squaredDistance > mMaximumPlayerDistanceSquared)
	{
		move = direction * mSpeed * deltaTime;
	}

	// update the scene object
	Spatial* pSpatial = DynamicCast<Spatial>(GetSceneObject());
	WIRE_ASSERT(pSpatial);
	pSpatial->Local.SetTranslate(probeRobotPosition);
	pSpatial->Local.SetRotate(rotation);

	// update the physics object
	mpPhysicsEntity->setWalkDirection(PhysicsWorld::Convert(move));
}
