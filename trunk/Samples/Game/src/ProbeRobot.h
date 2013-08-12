#ifndef PROBEROBOTCONTROLLER_H
#define PROBEROBOTCONTROLLER_H

#include "PhysicsController.h"
#include "WireSpatial.h"
#include "PhysicsWorld.h" // TODO

class ProbeRobot : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	ProbeRobot(Wire::Spatial* pPlayerSpatial, Wire::Spatial* pHealthBar);

	virtual Bool Update(Double appTime);
	virtual void OnAttach();
	void TakeDamage(Float damage);

private:
	void CalculateMovementAndRotation();
	void Die();

	Wire::SpatialPtr mspPlayerSpatial;
	Wire::SpatialPtr mspHealthBar;
	Wire::Vector3F mHealthBarScale;

	Float mTotalHealth;
	Float mHealth;
	Float mSpeed;
	Float mMaximumPlayerDistanceSquared;

	CharacterControllerPtr mspCharacter;
};

#endif
