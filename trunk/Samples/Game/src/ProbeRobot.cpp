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
	if (mHealth <= 0)
	{
		Die();
		return true;
	}

	CalculateMovementAndRotation();

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
	if (!GetSceneObject())
	{
		return;
	}

	// DEBUG:
	//System::Print("Probe robot has died\n");
	
	GetSceneObject()->DetachController(this);
	mspCharacter->Get()->setUserPointer(NULL);
}

//----------------------------------------------------------------------------
void ProbeRobot::Register(btDynamicsWorld* pPhysicsWorld)
{
	mspCharacter = GetSceneObject()->GetController<CharacterController>();

	// Add a reference to the controller in the physics object
	mspCharacter->Get()->setUserPointer(this);
}

//----------------------------------------------------------------------------
void ProbeRobot::TakeDamage(Float damage)
{
	// DEBUG:
	//System::Print("Probe robot has taken %.1f point of damage\n", damage);

	mHealth -= damage;
}

//----------------------------------------------------------------------------
void ProbeRobot::CalculateMovementAndRotation()
{
	btVector3 origin = mspCharacter->Get()->getWorldTransform().getOrigin();
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

	Vector3F move = Vector3F::ZERO;
	if (squaredDistance > mMaximumPlayerDistanceSquared)
	{
		move = direction * mSpeed * 1/60.0F;	// physics time step, TODO: get from physics world;
	}

	// update the scene object
	Spatial* pSpatial = DynamicCast<Spatial>(GetSceneObject());
	WIRE_ASSERT(pSpatial);
	Matrix34F matrix(Vector3F::UNIT_Y, angle, probeRobotPosition);
	pSpatial->Local.SetMatrix(matrix, false);

	// update the physics object
	mspCharacter->GetCharacter()->setWalkDirection(PhysicsWorld::Convert(move));
}
