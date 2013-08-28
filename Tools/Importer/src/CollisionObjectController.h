#pragma once
#ifndef COLLISIONOBJECTCONTROLLER_H
#define COLLISIONOBJECTCONTROLLER_H

#include "btBulletDynamicsCommon.h"
#include "PhysicsController.h"
#include "WireNode.h"

class PhysicsWorld;

class CollisionObjectController : public PhysicsController
{
	WIRE_DECLARE_RTTI;

public:
	CollisionObjectController(PhysicsWorld* pPhysicsWorld, btCollisionObject* pCollisionObject);
	virtual ~CollisionObjectController();

	inline btCollisionObject* Get() { return mpCollisionObject; }

	// add/remove from the physics world
	virtual Bool SetEnabled(Bool isEnabled);

	virtual void ToggleDebugShape(Bool show = true, Bool destroyOnHide = false);

	// geometric update called by UpdateGS()
	virtual Bool Update(Double appTime);

protected:
	friend class PhysicsWorld;
	virtual void Unbind();

	btCollisionObject* mpCollisionObject;

	Wire::NodePtr mspDebugShape;
};

typedef Wire::Pointer<CollisionObjectController> CollisionObjectControllerPtr;

#endif
