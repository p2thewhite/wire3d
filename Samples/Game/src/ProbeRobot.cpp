#include "ProbeRobot.h"

#include "PhysicsWorld.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(ProbeRobot, PhysicsController);

//----------------------------------------------------------------------------
ProbeRobot::ProbeRobot(PhysicsWorld* pPhysicsWorld, Spatial* pPlayerSpatial,
	Spatial* pHealthBar)
	:
	PhysicsController(pPhysicsWorld),
	mspPlayerSpatial(pPlayerSpatial),
	mspHealthBar(pHealthBar),
	mTotalHealth(100.0F),
	mHealth(100.0F),
	mSpeed(7.5F),
	mMaximumPlayerDistanceSquared(10*10)
{
	WIRE_ASSERT(mspPlayerSpatial);
	WIRE_ASSERT(mspHealthBar);

	mHealthBarScale = mspHealthBar->Local.GetScale();
	SetEnabled(true);
}

//----------------------------------------------------------------------------
ProbeRobot::~ProbeRobot()
{
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
	mspCharacter->GetCharacter()->setWalkDirection(btVector3(0, 0, 0));
}

//----------------------------------------------------------------------------
void ProbeRobot::OnAttach()
{
	mspCharacter = GetSceneObject()->GetController<CharacterController>();
	WIRE_ASSERT(mspCharacter);

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
	Vector3F direction = playerPosition - probeRobotPosition;
	Vector3F distance = direction;
	distance.Y() = 0;
	direction.Normalize();

	Vector3F move = Vector3F::ZERO;
	Float distanceSquared = distance.SquaredLength();
	if (distanceSquared > mMaximumPlayerDistanceSquared)
	{
		distanceSquared -= mMaximumPlayerDistanceSquared;
		const Float range = 60;
		distanceSquared = MathF::Min(distanceSquared, range);
		Float speed = mSpeed * distanceSquared / range;
		if (mpPhysicsWorld)
		{
			move = direction * speed * static_cast<Float>(mpPhysicsWorld->
				GetFixedTimeStep());
		}
	}

	// update the scene object
	Spatial* pSpatial = DynamicCast<Spatial>(GetSceneObject());
	WIRE_ASSERT(pSpatial);
	distance.Normalize();

	Vector3F right = distance.Cross(Vector3F::UNIT_Y);
	right.Normalize();
	Matrix34F matrix(-right, Vector3F::UNIT_Y, distance, probeRobotPosition);
	pSpatial->Local.SetMatrix(matrix, false);

	// update the physics object
	mspCharacter->GetCharacter()->setWalkDirection(PhysicsWorld::Convert(move));
}
