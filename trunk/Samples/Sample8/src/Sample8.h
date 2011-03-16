#pragma once
#ifndef SAMPLE8_H
#define SAMPLE8_H

#include "WireApplication.h"
#include "btBulletDynamicsCommon.h"

using namespace Wire;

class Sample8 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	Sample8();
	virtual Bool OnInitialize();
	virtual void OnIdle();
	virtual void OnTerminate();

private:
	void CreatePhysics();
	void DestroyPhysics();
	void UpdatePhysics(btScalar elapsedTime);

	inline btVector3 ToBtVector3(Vector3F in)
	{
		return btVector3(in.X(), in.Y(), in.Z());
	}

	inline void ToWireQuaternion(const btQuaternion& in, QuaternionF& out)
	{
		out = QuaternionF(in.getW(), in.getX(), in.getY(), in.getZ());
	}

	Culler mCuller;
	TArray<GeometryPtr> mCubes;
	CameraPtr mspCamera;
	Float mAngle;
	Double mLastTime;

	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btAxisSweep3* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpSolver;
	btDiscreteDynamicsWorld* mpDynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> mCollisionShapes;
};

WIRE_REGISTER_INITIALIZE(Sample8);

#endif
