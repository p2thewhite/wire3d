#include "CharacterController.h"

#include "PhysicsWorld.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(CharacterController, CollisionObjectController);

//----------------------------------------------------------------------------
CharacterController::CharacterController(PhysicsWorld* pPhysicsWorld,
	btCollisionObject* pGhost, btKinematicCharacterController* pCharacter)
	:
	CollisionObjectController(pPhysicsWorld, pGhost),
	mpCharacter(pCharacter)
{
}

//----------------------------------------------------------------------------
CharacterController::~CharacterController()
{
	WIRE_DELETE mpCharacter;
}
