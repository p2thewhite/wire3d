#include "RigidBodyController.h"

#include "PhysicsWorld.h"
#include "WireIndexBuffer.h"
#include "WireQuaternion.h"
#include "WireSpatial.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(RigidBodyController, Controller);

//----------------------------------------------------------------------------
RigidBodyController::RigidBodyController(PhysicsWorld* pPhysicsWorld,
	btRigidBody* pRigidBody)
	:
	mpPhysicsWorld(pPhysicsWorld),
	mpRigidBody(pRigidBody)
{
}

//----------------------------------------------------------------------------
RigidBodyController::~RigidBodyController()
{
	if (mpPhysicsWorld)
	{
		mpPhysicsWorld->RemoveController(this);
	}
}

//----------------------------------------------------------------------------
void RigidBodyController::Unbind()
{
	if (mpPhysicsWorld)
	{
		mpPhysicsWorld->RemoveController(this);
		mpPhysicsWorld = NULL;
	}

	mpRigidBody = NULL;
}

//----------------------------------------------------------------------------
Bool RigidBodyController::Update(Double appTime)
{
	WIRE_ASSERT(DynamicCast<Spatial>(mpSceneObject));
	if (!mpRigidBody || !Controller::Update(appTime))
	{
		return false;
	}

	const Bool isStatic = mpRigidBody->isStaticObject();
	if (mpRigidBody->getMotionState() && (!isStatic || mspDebugShape)) 
	{
		btTransform trans;
		mpRigidBody->getMotionState()->getWorldTransform(trans);
		Vector3F pos = PhysicsWorld::Convert(trans.getOrigin());
		QuaternionF quat = PhysicsWorld::Convert(trans.getRotation());
		Matrix3F mat;
		quat.ToRotationMatrix(mat);

		if (!isStatic)
		{
			WIRE_ASSERT(DynamicCast<Spatial>(mpSceneObject));
			Spatial* pSpatial = StaticCast<Spatial>(mpSceneObject);
			pSpatial->World.SetTranslate(pos);
			pSpatial->World.SetRotate(mat);
			pSpatial->WorldIsCurrent = true;
		}

		if (mspDebugShape)
		{
			mspDebugShape->World.SetTranslate(pos);
			mspDebugShape->World.SetRotate(mat);
			mspDebugShape->WorldIsCurrent = true;
		}
	}

	return true;
}

//----------------------------------------------------------------------------
void RigidBodyController::ToggleDebugShape(Bool show, Bool destroyOnHide)
{
	if (!mspDebugShape && show && mpRigidBody && mpPhysicsWorld)
	{
		mspDebugShape = mpPhysicsWorld->CreateDebugShape(mpRigidBody->
			getCollisionShape());
	}

	Node* pOwner = DynamicCast<Node>(mpSceneObject);
	if (pOwner && mspDebugShape)
	{
		pOwner->DetachChild(mspDebugShape);
		if (show)
		{
			pOwner->AttachChild(mspDebugShape);

			// TODO: fix this
			Spatial* pRoot = pOwner;
			while (pRoot->GetParent())
			{
				pRoot = pRoot->GetParent();
			}

			pRoot->UpdateRS();
			pOwner->WorldBoundIsCurrent = false;
		}
		else if (destroyOnHide)
		{
			mspDebugShape = NULL;
		}
	}
}
