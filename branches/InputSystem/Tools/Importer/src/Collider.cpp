#include "Collider.h"
#include "WireSpatial.h"
#include "BulletUtils.h"
#include "WireSystem.h"

using namespace Wire;

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

	Spatial* pSpatial = DynamicCast<Spatial>(mpObject);

	WIRE_ASSERT(pSpatial);

	// Setting position
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(BulletUtils::Convert(pSpatial->Local.GetTranslate()));

	// Setting rotation
	transform.setBasis(BulletUtils::Convert(pSpatial->Local.GetRotate()));

	// Setting scale
	mpShape->setLocalScaling(BulletUtils::Convert(pSpatial->Local.GetScale()));
	mpShape->setMargin(0.5f);

	btDefaultMotionState* pMotionState = WIRE_NEW btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInformation(0, pMotionState, mpShape, btVector3(0, 0, 0));
	btRigidBody* pRigidBody = WIRE_NEW btRigidBody(rigidBodyInformation);

	// Only do CCD if  motion in one timestep (1/60) exceeds 
	pRigidBody->setCcdMotionThreshold(0.5f);

	//Experimental: better estimation of CCD Time of Impact.
	pRigidBody->setCcdSweptSphereRadius(0.2f);

	pPhysicsWorld->addRigidBody(pRigidBody);
}

//----------------------------------------------------------------------------
Bool Collider::Update(Double appTime)
{
	// TODO: create some kind of collider callback, maybe?
	return true;
}
