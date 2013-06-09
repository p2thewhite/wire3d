#include "PhysicsWorld.h"

#include "WireIndexBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(PhysicsWorld, Object);

//----------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld(const Vector3F& rWorldAabbMin, const Vector3F&
	rWorldAabbMax, const Vector3F& rGravity)
	:
	mCollisionShapes(10, 10)
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
	for (UInt i = 0; i < mCollisionShapes.GetQuantity(); i++)
	{
		btCollisionShape* pShape = mCollisionShapes[i].CollisionShape;
		if (pShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE &&
			!mCollisionShapes[i].ReferencedObject0 &&
			!mCollisionShapes[i].ReferencedObject1)
		{
			btBvhTriangleMeshShape* pMeshShape = static_cast<btBvhTriangleMeshShape*>(pShape);
			btTriangleIndexVertexArray* pMeshInterface = static_cast<btTriangleIndexVertexArray*>(
				pMeshShape->getMeshInterface());
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
		mCollisionShapes[i] = CollisionShapeItem();
	}

	WIRE_DELETE mpDynamicsWorld;
	WIRE_DELETE mpSolver;
	WIRE_DELETE mpOverlappingPairCache;
	WIRE_DELETE mpDispatcher;
	WIRE_DELETE mpCollisionConfiguration;
}

//----------------------------------------------------------------------------
Int PhysicsWorld::StepSimulation(Double deltaTime, Int maxSubSteps,
	Double fixedTimeStep)
{
	mFixedTimeStep = fixedTimeStep;
	return mpDynamicsWorld->stepSimulation(btScalar(deltaTime), maxSubSteps,
		btScalar(fixedTimeStep));
}

//----------------------------------------------------------------------------
void PhysicsWorld::AddRigidBody(btRigidBody* pRigidBody)
{
	mpDynamicsWorld->addRigidBody(pRigidBody);
}

//----------------------------------------------------------------------------
void PhysicsWorld::AddCollisionShape(btCollisionShape* pShape, Object*
	pReferencedObject0, Object* pReferencedObject1)
{
	CollisionShapeItem item;
	item.CollisionShape = pShape;
	item.ReferencedObject0 = pReferencedObject0;
	item.ReferencedObject1 = pReferencedObject1;
	mCollisionShapes.Append(item);
}

//----------------------------------------------------------------------------
RigidBodyController* PhysicsWorld::GetController(btRigidBody* pRigidBody)
{
	RigidBodyController** pValue =	mControllerMap.Find(pRigidBody);
	if (pValue)
	{
		return *pValue;
	}

	return NULL;
}

//----------------------------------------------------------------------------
RigidBodyController* PhysicsWorld::AddController(btRigidBody* pRigidBody)
{
	WIRE_ASSERT(mControllerMap.Find(pRigidBody) == NULL);

	RigidBodyController* pController = WIRE_NEW RigidBodyController(this,
		pRigidBody);
	mControllerMap.Insert(pRigidBody, pController);
	return pController;
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
	Bool found = false;
	for (UInt i = 0; i < mCollisionShapes.GetQuantity(); i++)
	{
		if (mCollisionShapes[i].CollisionShape == pShape)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		CollisionShapeItem item(pShape);
		mCollisionShapes.Append(item);
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
btTriangleIndexVertexArray* PhysicsWorld::Convert(Mesh* pMesh, Bool copy)
{
	IndexBuffer* pIndexBuffer = pMesh->GetIndexBuffer();
	UShort* pTriangleIndexBase = pIndexBuffer->GetData();
	VertexBuffer* pVertexBuffer = pMesh->GetPositionBuffer();
	Vector3F* pVertexBase = &(pVertexBuffer->Position3(0));
	const VertexAttributes& rAttr = pVertexBuffer->GetAttributes();
	Int vertexStride = rAttr.GetChannelQuantity() * sizeof(Float);

	if (copy)
	{
		pTriangleIndexBase = WIRE_NEW UShort[pIndexBuffer->GetQuantity()];
		UInt size = pIndexBuffer->GetQuantity() * sizeof(UShort);
		System::Memcpy(pTriangleIndexBase, size, pIndexBuffer->GetData(), size);

		pVertexBase = WIRE_NEW Vector3F[pVertexBuffer->GetQuantity()];
		size = pVertexBuffer->GetQuantity() * sizeof(Vector3F);

		if (rAttr.GetChannelQuantity() == rAttr.GetPositionChannels())
		{
			System::Memcpy(pVertexBase, size, pVertexBuffer->GetPosition(), size);
		}
		else
		{
			vertexStride = sizeof(Vector3F);
			for (UInt i = 0; i < pVertexBuffer->GetQuantity(); i++)
			{
				pVertexBase[i] = pVertexBuffer->Position3(i);
			}
		}
	}

	btIndexedMesh indexedMesh;
	WIRE_ASSERT((pIndexBuffer->GetQuantity() % 3) == 0);
	indexedMesh.m_numTriangles = pIndexBuffer->GetQuantity() / 3;
	indexedMesh.m_triangleIndexBase = reinterpret_cast<const UChar*>(
		pTriangleIndexBase);
	indexedMesh.m_triangleIndexStride = sizeof(UShort) * 3;
	indexedMesh.m_numVertices = pVertexBuffer->GetQuantity();
	indexedMesh.m_vertexBase = reinterpret_cast<const UChar*>(pVertexBase);
	indexedMesh.m_vertexStride = vertexStride;

	btTriangleIndexVertexArray* pTriangleIndexVertexArray =
		WIRE_NEW btTriangleIndexVertexArray();
	pTriangleIndexVertexArray->addIndexedMesh(indexedMesh, PHY_SHORT);
	return pTriangleIndexVertexArray;
}
