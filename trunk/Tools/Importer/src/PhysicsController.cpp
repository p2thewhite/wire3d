#include "PhysicsController.h"

#include "PhysicsWorld.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(PhysicsController, Wire::Controller);

//----------------------------------------------------------------------------
PhysicsController::PhysicsController(PhysicsWorld* pPhysicsWorld)
	:
	mpPhysicsWorld(pPhysicsWorld),
	mIsEnabled(false)
{
	WIRE_ASSERT(mpPhysicsWorld);
}

//----------------------------------------------------------------------------
PhysicsController::~PhysicsController()
{
	if (mpPhysicsWorld)
	{
		mpPhysicsWorld->RemoveController(this);
		mpPhysicsWorld = NULL;
	}
}

//----------------------------------------------------------------------------
Bool PhysicsController::SetEnabled(Bool isEnabled)
{
	if (mIsEnabled == isEnabled)
	{
		return false;
	}

	mIsEnabled = isEnabled;
	if (!mpPhysicsWorld)
	{
		return false;
	}

	if (mIsEnabled)
	{
		mpPhysicsWorld->AddController(this);
	}
	else
	{
		mpPhysicsWorld->RemoveController(this, false);
	}

	return true;
}

//----------------------------------------------------------------------------
void PhysicsController::Unbind()
{
	if (mpPhysicsWorld)
	{
		mpPhysicsWorld->RemoveController(this);
		mpPhysicsWorld = NULL;
	}
}
