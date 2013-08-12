#include "CollisionObjectController.h"

#include "PhysicsWorld.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(CollisionObjectController, PhysicsController);

//----------------------------------------------------------------------------
CollisionObjectController::CollisionObjectController(PhysicsWorld* pPhysicsWorld,
	btCollisionObject* pCollisionObject)
	:
	PhysicsController(pPhysicsWorld),
	mpCollisionObject(pCollisionObject)
{
}

//----------------------------------------------------------------------------
CollisionObjectController::~CollisionObjectController()
{
	if (mpPhysicsWorld)
	{
		mpPhysicsWorld->RemoveController(this);
	}
}

//----------------------------------------------------------------------------
void CollisionObjectController::Unbind()
{
	PhysicsController::Unbind();

	mpCollisionObject = NULL;
}

//----------------------------------------------------------------------------
void CollisionObjectController::ToggleDebugShape(Bool show, Bool destroyOnHide)
{
	if (!mspDebugShape && show && mpCollisionObject && mpPhysicsWorld)
	{
		mspDebugShape = mpPhysicsWorld->CreateDebugShape(mpCollisionObject->
			getCollisionShape());
	}

	Node* pOwner = DynamicCast<Node>(mpSceneObject);
	if (pOwner && mspDebugShape)
	{
		pOwner->DetachChild(mspDebugShape);
		if (show)
		{
			pOwner->AttachChild(mspDebugShape);
			pOwner->WorldBoundIsCurrent = false;
		}
		else if (destroyOnHide)
		{
			mspDebugShape = NULL;
		}
	}
}

//----------------------------------------------------------------------------
Bool CollisionObjectController::Update(Double appTime)
{
	if (!mpCollisionObject || !Controller::Update(appTime))
	{
		return false;
	}

	const Bool isStatic = mpCollisionObject->isStaticObject();
	if (!isStatic || mspDebugShape)
	{
		btTransform trans =	mpCollisionObject->getWorldTransform();
		Vector3F pos = PhysicsWorld::Convert(trans.getOrigin());
		QuaternionF quat = PhysicsWorld::Convert(trans.getRotation());
		Matrix3F mat;
		quat.ToRotationMatrix(mat);

		if (mspDebugShape)
		{
			mspDebugShape->World.SetTranslate(pos);
			mspDebugShape->World.SetRotate(mat);
			mspDebugShape->WorldIsCurrent = true;
		}
	}

	return true;
}
