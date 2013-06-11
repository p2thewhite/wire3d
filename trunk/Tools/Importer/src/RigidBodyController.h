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

	void ToggleDebugShape(Bool show = true, Bool destroyOnHide = false,
		const Wire::Color32& rColor = Wire::Color32(127, 255, 127, 255));

	static Wire::RenderObject* CreateDebugShape(btCollisionShape* pShape,
		const Wire::Color32& rColor);

private:
	friend class PhysicsWorld;
	void Unbind();

	static Wire::RenderObject* CreateDebugBox(btBoxShape* pBox, const Wire::
		Color32& rColor);
	static Wire::RenderObject* CreateDebugSphere(btSphereShape* pSphereShape,
		const Wire::Color32& rColor);
	static Wire::RenderObject* CreateDebugCapsule(btCapsuleShape*
		pCapsuleShape, const Wire::Color32& rColor);
	static Wire::RenderObject* CreateDebugMesh(btStridingMeshInterface*
		pMeshInterface, const Wire::Color32& rColor);
// 	static Wire::RenderObject* CreateDebugHull(btConvexHullShape*
// 		pConvexHullShape, const Wire::Color32& rColor);

	PhysicsWorld* mpPhysicsWorld;
	btRigidBody* mpRigidBody;

	Wire::NodePtr mspDebugShape;
};

typedef Wire::Pointer<RigidBodyController> RigidBodyControllerPtr;

#endif
