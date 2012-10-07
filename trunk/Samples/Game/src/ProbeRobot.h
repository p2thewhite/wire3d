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
	ProbeRobot(Wire::Spatial* pPlayerSpatial, Wire::Spatial* pHealthBar);

	virtual Bool Update(Double appTime);
	void Register(btDynamicsWorld* pPhysicsWorld);
	void TakeDamage(Float damage);

private:
	void CalculateMovementAndRotation(Float deltaTime);
	void Die();

	Wire::SpatialPtr mspPlayerSpatial;
	Wire::SpatialPtr mspHealthBar;
	Wire::Vector3F mHealthBarScale;

	Float mTotalHealth;
	Float mHealth;
	Float mSpeed;
	Float mMaximumPlayerDistanceSquared;

	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsEntity;
};

#endif
