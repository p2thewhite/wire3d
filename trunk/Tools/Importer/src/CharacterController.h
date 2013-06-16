#pragma once
#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "CollisionObjectController.h"

class PhysicsWorld;
class btKinematicCharacterController;

class CharacterController : public CollisionObjectController
{
	WIRE_DECLARE_RTTI;

public:
	CharacterController(PhysicsWorld* pPhysicsWorld, btCollisionObject* pGhost,
		btKinematicCharacterController* pCharacter);
	virtual ~CharacterController();

	inline btKinematicCharacterController* GetCharacter() { return mpCharacter; }

private:
	btKinematicCharacterController* mpCharacter;
};

typedef Wire::Pointer<CharacterController> CharacterControllerPtr;

#endif
