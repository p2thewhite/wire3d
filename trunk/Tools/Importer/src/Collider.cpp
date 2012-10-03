#include "Collider.h"

#include "BulletUtils.h"
#include "WireSpatial.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Collider, Controller);

//----------------------------------------------------------------------------
Collider::Collider(btCollisionShape* pShape)
	: mpShape(pShape)
{
}

//----------------------------------------------------------------------------
Collider::~Collider()
{
}

//----------------------------------------------------------------------------
void Collider::Register(btDynamicsWorld* pPhysicsWorld)
{
	WIRE_ASSERT(pPhysicsWorld);

	Spatial* pSpatial = DynamicCast<Spatial>(mpSceneObject);
	WIRE_ASSERT(pSpatial);

	// Setting position
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(BulletUtils::Convert(pSpatial->Local.GetTranslate()));

	// Setting rotation
	transform.setBasis(BulletUtils::Convert(pSpatial->Local.GetRotate()));

	// Setting scale
	mpShape->setLocalScaling(BulletUtils::Convert(pSpatial->Local.GetScale()));
	mpShape->setMargin(0.5F);

	btDefaultMotionState* pMotionState = WIRE_NEW btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInformation(0, pMotionState,
		mpShape, btVector3(0, 0, 0));
	btRigidBody* pRigidBody = WIRE_NEW btRigidBody(rigidBodyInformation);

	// Only do CCD if  motion in one timestep (1/60) exceeds 
	pRigidBody->setCcdMotionThreshold(0.5F);

	//Experimental: better estimation of CCD Time of Impact.
	pRigidBody->setCcdSweptSphereRadius(0.2F);

	pPhysicsWorld->addRigidBody(pRigidBody);
}
