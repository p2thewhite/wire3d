#pragma once
#ifndef COLLISIONOBJECTCONTROLLER_H
#define COLLISIONOBJECTCONTROLLER_H

#include "btBulletDynamicsCommon.h"
#include "WireController.h"
#include "WireNode.h"

class PhysicsWorld;

class CollisionObjectController : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	CollisionObjectController(PhysicsWorld* pPhysicsWorld, btCollisionObject* pCollisionObject);
	virtual ~CollisionObjectController();

	inline btCollisionObject* Get() { return mpCollisionObject; }

	void ToggleDebugShape(Bool show = true, Bool destroyOnHide = false);

	// geometric update called by UpdateGS()
	virtual Bool Update(Double appTime);

protected:
	friend class PhysicsWorld;
	void Unbind();

	PhysicsWorld* mpPhysicsWorld;
	btCollisionObject* mpCollisionObject;

	Wire::NodePtr mspDebugShape;
};

typedef Wire::Pointer<CollisionObjectController> CollisionObjectControllerPtr;

#endif
