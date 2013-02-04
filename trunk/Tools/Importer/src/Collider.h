#pragma once
#ifndef COLLIDER_H__
#define COLLIDER_H__

#include "WireController.h"
#include "WireVector3.h"
#include "btBulletDynamicsCommon.h"

class Collider : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	Collider(btCollisionShape* pShape, btDefaultMotionState* pMotionState);
	~Collider();

	Wire::Vector3F GetWorldTranslate();

	inline btCollisionShape* GetShape()
	{
		return mpShape;
	}

private:
	btCollisionShape* mpShape;
	btDefaultMotionState* mpMotionState;
};

#endif
