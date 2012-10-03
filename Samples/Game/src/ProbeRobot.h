#ifndef PROBEROBOTCONTROLLER_H
#define PROBEROBOTCONTROLLER_H

#include "WireController.h"
#include "WireSpatial.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class ProbeRobot : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	ProbeRobot(Wire::Spatial* pPlayerSpatial);

	virtual Bool Update(Double appTime);
	void Register(btDynamicsWorld* pPhysicsWorld);
	void SetTotalHealth(Float totalHealth);
	Float GetTotalHealth();
	Float GetHealth();
	void TakeDamage(Float damage);

private:
	Float GetDeltaTime(Double appTime);
	void CalculateMovementAndRotation(Float deltaTime);
	Wire::Vector3F GetPosition();
	void UpdateModel();
	void MovePhysicsEntity();
	void Die();

	Wire::SpatialPtr mspPlayerSpatial;
	Float mTotalHealth;
	Float mHealth;
	Float mSpeed;
	Float mMaximumPlayerDistanceSquared;
	Wire::Vector3F mMove;
	Wire::Matrix3F mRotation;

	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsEntity;
};

#endif
