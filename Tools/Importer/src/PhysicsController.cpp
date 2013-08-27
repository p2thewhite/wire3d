#include "PhysicsController.h"

#include "PhysicsWorld.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(PhysicsController, Wire::Controller);

//----------------------------------------------------------------------------
PhysicsController::PhysicsController(PhysicsWorld* pPhysicsWorld)
	:
	mpPhysicsWorld(pPhysicsWorld)
{
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
void PhysicsController::Unbind()
{
	if (mpPhysicsWorld)
	{
		mpPhysicsWorld->RemoveController(this);
		mpPhysicsWorld = NULL;
	}
}
