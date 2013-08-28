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
	// TODO: fix (parent) destructor if !mIsEnabled
	WIRE_DELETE mpCharacter;
}

//----------------------------------------------------------------------------
Bool CharacterController::SetEnabled(Bool isEnabled)
{
	if (CollisionObjectController::SetEnabled(isEnabled))
	{
		if (mIsEnabled)
		{
			mpPhysicsWorld->Get()->addCollisionObject(mpCollisionObject,
				btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | 
				btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter);
			mpPhysicsWorld->Get()->addAction(mpCharacter);
		}
		else
		{
			mpPhysicsWorld->Get()->removeAction(mpCharacter);
		}

		return true;
	}

	return false;
}
