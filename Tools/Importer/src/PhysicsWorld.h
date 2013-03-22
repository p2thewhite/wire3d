#pragma once
#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "btBulletDynamicsCommon.h"
#include "RigidBodyController.h"
#include "WireColor32.h"
#include "WireMatrix3.h"
#include "WireObject.h"
#include "WireQuaternion.h"
#include "WireRenderObject.h"
#include "WireTHashTable.h"
#include "WireVector3.h"

class PhysicsWorld : public Wire::Object
{
	WIRE_DECLARE_RTTI;

public:
	PhysicsWorld(
		const Wire::Vector3F& rWorldAabbMin = Wire::Vector3F(-100, -100, -100),
		const Wire::Vector3F& rWorldAabbMax = Wire::Vector3F(100, 100, 100),
		const Wire::Vector3F& rGravity = Wire::Vector3F(0, -9.81F, 0));
	virtual ~PhysicsWorld();

	inline btDynamicsWorld* Get();

	void AddRigidBody(btRigidBody* pRigidBody);
	RigidBodyController* GetController(btRigidBody* pRigidBody);
	void RemoveController(RigidBodyController* pController,
		Bool destroyRigidBody = true);

	void ToggleDebugShapes(Bool show = true, Bool destroyOnHide = false,
		const Wire::Color32& rColor = Wire::Color32(127, 255, 127, 255));

	static inline btVector3 Convert(const Wire::Vector3F& rIn);
	static inline Wire::Vector3F Convert(const btVector3& rIn);
	static inline btMatrix3x3 Convert(const Wire::Matrix3F& rIn);
	static inline Wire::QuaternionF Convert(const btQuaternion& rIn);

	static btTriangleIndexVertexArray* Convert(Wire::Mesh* pMesh);

private:
	void DestroyCollisionObject(btCollisionObject* pCollisionObject);

	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btAxisSweep3* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpSolver;
	btDiscreteDynamicsWorld* mpDynamicsWorld;

	// Keep track of the collision shapes, we release memory at exit.
	btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

	Wire::THashTable<btRigidBody*, RigidBodyController*> mControllerMap;
};

typedef Wire::Pointer<PhysicsWorld> PhysicsWorldPtr;
#include "PhysicsWorld.inl"

#endif
