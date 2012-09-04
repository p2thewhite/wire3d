#pragma once
#ifndef COLLIDER_H__
#define COLLIDER_H__

#include "WireController.h"
#include "btBulletDynamicsCommon.h"

class Collider : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	Collider(btCollisionShape* pShape);
	~Collider();

	inline btCollisionShape* GetShape()
	{
		return mpShape;
	}

	virtual void Register(btDynamicsWorld* pPhysicsWorld);

private:
	btCollisionShape* mpShape;
};

#endif
