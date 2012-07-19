#pragma once
#ifndef COLLIDER_H__
#define COLLIDER_H__

#include "WireController.h"
#include "WireVector3.h"

#include "btBulletDynamicsCommon.h"

using namespace Wire;

class Collider : public Controller
{
public:
	Collider(btCollisionShape* pShape);
	~Collider();

	inline btCollisionShape* GetShape()
	{
		return mpShape;
	}

	virtual void Register(btDynamicsWorld* pPhysicsWorld);
	virtual Bool Update(Double appTime);

private:
	btCollisionShape* mpShape;

};

#endif