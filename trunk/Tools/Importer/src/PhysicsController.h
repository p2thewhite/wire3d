#pragma once
#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H

#include "WireController.h"

class PhysicsWorld;

class PhysicsController : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	PhysicsController(PhysicsWorld* pPhysicsWorld);
	virtual ~PhysicsController();

 	// update called by PhysicsWorld::StepSimulation()
 	virtual void PhysicsUpdate(Double appTime) {}

protected:
	friend class PhysicsWorld;
	virtual void Unbind();

	PhysicsWorld* mpPhysicsWorld;
};

typedef Wire::Pointer<PhysicsController> PhysicsControllerPtr;

#endif
