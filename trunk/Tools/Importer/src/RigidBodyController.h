#pragma once
#ifndef RIGIDBODYCONTROLLER_H
#define RIGIDBODYCONTROLLER_H

#include "btBulletDynamicsCommon.h"
#include "WireController.h"
#include "WireNode.h"

class PhysicsWorld;

class RigidBodyController : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	RigidBodyController(PhysicsWorld* pPhysicsWorld, btRigidBody* pRigidBody);
	virtual ~RigidBodyController();

	// geometric update called by UpdateGS()
	virtual Bool Update(Double appTime);

	inline btRigidBody* Get() { return mpRigidBody; }

	void ToggleDebugShape(Bool show = true, Bool destroyOnHide = false);

private:
	friend class PhysicsWorld;
	void Unbind();

	PhysicsWorld* mpPhysicsWorld;
	btRigidBody* mpRigidBody;

	Wire::NodePtr mspDebugShape;
};

typedef Wire::Pointer<RigidBodyController> RigidBodyControllerPtr;

#endif
