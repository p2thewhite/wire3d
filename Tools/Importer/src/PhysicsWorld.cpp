#include "PhysicsWorld.h"

#include "WireIndexBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(PhysicsWorld, Object);

//----------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld(const Vector3F& rWorldAabbMin, const Vector3F&
	rWorldAabbMax, const Vector3F& rGravity)
{
	///collision configuration contains default setup for memory.
	// Advanced users can create their own configuration.
	mpCollisionConfiguration = WIRE_NEW btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you
	// can use a different dispatcher (see Extras/BulletMultiThreaded)
	mpDispatcher = WIRE_NEW btCollisionDispatcher(mpCollisionConfiguration);

	///the maximum size of the collision world. Make sure objects stay
	// within these boundaries
	///Don't make the world AABB size too large, it will harm simulation
	// quality and performance
	btVector3 worldAabbMin = Convert(rWorldAabbMin);
	btVector3 worldAabbMax = Convert(rWorldAabbMax);
	UShort maxProxies = 1024; // TODO: review
	mpOverlappingPairCache = WIRE_NEW btAxisSweep3(worldAabbMin, worldAabbMax,
		maxProxies);

	///the default constraint solver. For parallel processing you can use
	// a different solver (see Extras/BulletMultiThreaded)
	mpSolver = WIRE_NEW btSequentialImpulseConstraintSolver;

	mpDynamicsWorld = WIRE_NEW btDiscreteDynamicsWorld(mpDispatcher,
		mpOverlappingPairCache, mpSolver, mpCollisionConfiguration);

	mpDynamicsWorld->setGravity(Convert(rGravity));
}

//----------------------------------------------------------------------------
PhysicsWorld::~PhysicsWorld()
{
	//cleanup in the reverse order of creation/initialization
	
	while (mControllerMap.GetQuantity() > 0)
	{
		THashTable<btRigidBody*, RigidBodyController*>::Iterator it(&mControllerMap);
		btRigidBody* pKey = NULL;
		RigidBodyController** pValue = it.GetFirst(&pKey);
		WIRE_ASSERT(pValue && *pValue);
		(*pValue)->Unbind();
	}

	//remove the rigid bodies from the dynamics world and delete them
	for (Int i = mpDynamicsWorld->getNumCollisionObjects()-1; i >= 0; i--)
	{
		DestroyCollisionObject(mpDynamicsWorld->getCollisionObjectArray()[i]);
	}

	//delete collision shapes
	for (Int j = 0; j < mCollisionShapes.size(); j++)
	{
		btCollisionShape* pShape = mCollisionShapes[j];
		mCollisionShapes[j] = NULL;

		if (pShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
		{
			btBvhTriangleMeshShape* pMeshShape = static_cast<
				btBvhTriangleMeshShape*>(pShape);
			btTriangleIndexVertexArray* pMeshInterface = static_cast<
				btTriangleIndexVertexArray*>(pMeshShape->getMeshInterface());
			IndexedMeshArray& rMeshArray = pMeshInterface->getIndexedMeshArray();
			for (Int k = 0; k < rMeshArray.size(); k++)
			{
				WIRE_DELETE[] rMeshArray[k].m_triangleIndexBase;
				rMeshArray[k].m_triangleIndexBase = NULL;
				WIRE_DELETE[] rMeshArray[k].m_vertexBase;
				rMeshArray[k].m_vertexBase = NULL;
			}
		}

		WIRE_DELETE pShape;
	}

	WIRE_DELETE mpDynamicsWorld;
	WIRE_DELETE mpSolver;
	WIRE_DELETE mpOverlappingPairCache;
	WIRE_DELETE mpDispatcher;
	WIRE_DELETE mpCollisionConfiguration;
}

//----------------------------------------------------------------------------
void PhysicsWorld::AddRigidBody(btRigidBody* pRigidBody)
{
	mpDynamicsWorld->addRigidBody(pRigidBody);
}

//----------------------------------------------------------------------------
RigidBodyController* PhysicsWorld::GetController(btRigidBody* pRigidBody)
{
	RigidBodyController** pValue =	mControllerMap.Find(pRigidBody);
	if (!pValue)
	{
		RigidBodyController* pController = WIRE_NEW RigidBodyController(this,
			pRigidBody);
		mControllerMap.Insert(pRigidBody, pController);
		return pController;
	}

	return *pValue;
}

//----------------------------------------------------------------------------
void PhysicsWorld::RemoveController(RigidBodyController* pController,
	Bool destroyRigidBody)
{
	THashTable<btRigidBody*, RigidBodyController*>::Iterator it(
		&mControllerMap);
	btRigidBody* pKey = NULL;

	for (RigidBodyController** pValue = it.GetFirst(&pKey); pValue;
		pValue = it.GetNext(&pKey))
	{
		WIRE_ASSERT(pValue && *pValue);
		if (*pValue == pController)
		{
			mControllerMap.Remove(pKey);
			break;
		}
	}

	if (pKey && destroyRigidBody)
	{
		btCollisionObjectArray& rArray = mpDynamicsWorld->
			getCollisionObjectArray();
		if (rArray.findLinearSearch(pKey) != rArray.size())
		{
			DestroyCollisionObject(pKey);
			pKey = NULL;
		}
	}
}

//----------------------------------------------------------------------------
void PhysicsWorld::DestroyCollisionObject(btCollisionObject* pCollisionObject)
{
	btCollisionShape* pShape = pCollisionObject->getCollisionShape();
	if (mCollisionShapes.findLinearSearch(pShape) == mCollisionShapes.size())
	{
		mCollisionShapes.push_back(pShape);
	}

	btRigidBody* pBody = btRigidBody::upcast(pCollisionObject);
	if (pBody && pBody->getMotionState())
	{
		WIRE_DELETE pBody->getMotionState();
	}

	mpDynamicsWorld->removeCollisionObject(pCollisionObject);
	WIRE_DELETE pCollisionObject;
}

//----------------------------------------------------------------------------
void PhysicsWorld::ToggleDebugShapes(Bool show, Bool destroyOnHide,
	const Wire::Color32& rColor)
{
	THashTable<btRigidBody*, RigidBodyController*>::Iterator it(
		&mControllerMap);
	btRigidBody* pKey;

	for (RigidBodyController** pValue = it.GetFirst(&pKey); pValue;
		pValue = it.GetNext(&pKey))
	{
		WIRE_ASSERT(pValue && *pValue);
		(*pValue)->ToggleDebugShape(show, destroyOnHide, rColor);
	}
}

//----------------------------------------------------------------------------
btTriangleIndexVertexArray* PhysicsWorld::Convert(Mesh* pMesh)
{
	IndexBuffer* pIndexBuffer = pMesh->GetIndexBuffer();
	UShort* pTriangleIndexBase = WIRE_NEW UShort[pIndexBuffer->GetQuantity()];
	UInt size = pIndexBuffer->GetQuantity() * sizeof(UShort);
	System::Memcpy(pTriangleIndexBase, size, pIndexBuffer->GetData(), size);

	VertexBuffer* pVertexBuffer = pMesh->GetVertexBuffer();
	btScalar* pVertexBase = WIRE_NEW btScalar[pVertexBuffer->GetQuantity()*3];
	size = pVertexBuffer->GetQuantity() * sizeof(Vector3F);
	System::Memcpy(pVertexBase, size, pVertexBuffer->GetPosition(), size);

	btIndexedMesh indexedMesh;
	WIRE_ASSERT((pIndexBuffer->GetQuantity() % 3) == 0);
	indexedMesh.m_numTriangles = pIndexBuffer->GetQuantity() / 3;
	indexedMesh.m_triangleIndexBase = (const UChar*)pTriangleIndexBase;
	indexedMesh.m_triangleIndexStride = sizeof(UShort) * 3;
	indexedMesh.m_numVertices = pVertexBuffer->GetQuantity();
	indexedMesh.m_vertexBase = (const UChar*)pVertexBase;
	indexedMesh.m_vertexStride = sizeof(btScalar);

	btTriangleIndexVertexArray* pTriangleIndexVertexArray =
		WIRE_NEW btTriangleIndexVertexArray();
	pTriangleIndexVertexArray->addIndexedMesh(indexedMesh, PHY_SHORT);
	return pTriangleIndexVertexArray;
}
