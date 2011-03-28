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
	TextureEffect* CreateTextureEffect();
	void CreatePhysics();
	void CreateRigidBody(btCollisionShape* pColShape, Float mass,
		Vector3F position, Float extent = 1.0F);
	void ResetRigidBodies();
	void UpdatePhysics(btScalar elapsedTime);
	void DestroyPhysics();

	inline btVector3 Convert(Vector3F in)
	{
		return btVector3(in.X(), in.Y(), in.Z());
	}

	inline Vector3F Convert(btVector3& in)
	{
		return Vector3F(in.getX(), in.getY(), in.getZ());
	}

	inline QuaternionF Convert(const btQuaternion& in)
	{
		return QuaternionF(in.getW(), in.getX(), in.getY(), in.getZ());
	}

	Culler mCuller;
	TArray<GeometryPtr> mGeometries;
	CameraPtr mspCamera;
	StateMaterialPtr mspMaterial;
	LightPtr mspLightA;
	LightPtr mspLightB;
	Float mAngle;
	Double mLastTime;
	Double mResetTime;

	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btAxisSweep3* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpSolver;
	btDiscreteDynamicsWorld* mpDynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

	static const UInt smBoxCountX = 5;
	static const UInt smBoxCountY = 6;
};

WIRE_REGISTER_INITIALIZE(Sample8);

#endif
