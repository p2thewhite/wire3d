#ifndef PROBEROBOTCONTROLLER_H_
#define PROBEROBOTCONTROLLER_H_

#include "WireController.h"
#include "WireSpatial.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

using namespace Wire;

class ProbeRobot : public Controller
{
	WIRE_DECLARE_RTTI;

public:
	ProbeRobot(Spatial* pPlayerSpatial);

	virtual Bool Update(Double appTime);
	void Register(btDynamicsWorld* pPhysicsWorld);
	void SetTotalHealth(Float totalHealth);
	Float GetTotalHealth();
	Float GetHealth();
	void SetSpeed(Float speed);
	void SetMaximumPlayerDistance(Float maximumPlayerDistance);
	void TakeDamage(Float damage);

private:
	Spatial* mpSpatial;
	SpatialPtr mspPlayerSpatial;
	Float mTotalHealth;
	Float mHealth;
	Float mSpeed;
	Float mSquaredMaximumPlayerDistance;
	Vector3F mMove;
	Matrix3F mStartingRotation;
	Matrix3F mRotation;
	btDynamicsWorld* mpPhysicsWorld;
	btPairCachingGhostObject* mpGhostObject;
	btKinematicCharacterController* mpPhysicsEntity;

	void InitializeIfNecessary();
	Vector3F GetPosition();
	void UpdateSpatial();
	void MovePhysicsEntity();
	void Die();

};

#endif
