#include "RigidBodyController.h"

#include "PhysicsWorld.h"
#include "WireIndexBuffer.h"
#include "WireQuaternion.h"
#include "WireSpatial.h"
#include "WireStandardMesh.h"
#include "WireStateWireframe.h"

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
void RigidBodyController::ToggleDebugShape(Bool show, Bool destroyOnHide,
	const Wire::Color32& rColor)
{
	if (!mspDebugShape && show)
	{
		RenderObject* pRenderObject = CreateDebugShape(mpRigidBody->
			getCollisionShape(), rColor);

		mspDebugShape = WIRE_NEW Node(pRenderObject);
		StateWireframe* pWireframe = WIRE_NEW StateWireframe;
		pWireframe->Enabled = true;
		mspDebugShape->AttachState(pWireframe);
		mspDebugShape->UpdateRS();
	}

	Node* pOwner = DynamicCast<Node>(mpSceneObject);
	WIRE_ASSERT(pOwner);
	if (pOwner)
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
RenderObject* RigidBodyController::CreateDebugShape(btCollisionShape* pShape,
	const Color32& rColor)
{
	if (pShape->getShapeType() == BOX_SHAPE_PROXYTYPE)
	{
		return CreateDebugBox(static_cast<btBoxShape*>(pShape), rColor);
	}
	else if (pShape->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
	{
		return CreateDebugSphere(static_cast<btSphereShape*>(pShape), rColor);
	}
	else if (pShape->getShapeType() == CAPSULE_SHAPE_PROXYTYPE)
	{
		return CreateDebugCapsule(static_cast<btCapsuleShape*>(pShape),
			rColor);
	}
	else if (pShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
	{
		return CreateDebugMesh(static_cast<btBvhTriangleMeshShape*>(pShape)->
			getMeshInterface(), rColor);
	}
	else if (pShape->getShapeType() == CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE)
	{
		return CreateDebugMesh(static_cast<btConvexTriangleMeshShape*>(
			pShape)->getMeshInterface(), rColor);
	}

	WIRE_ASSERT(false /* collision shape not supported */);
	return NULL;
}

//----------------------------------------------------------------------------
RenderObject* RigidBodyController::CreateDebugBox(btBoxShape* pBox, const
	Color32& rColor)
{
	// Resetting margin to acquire the real half extents
	Float margin = pBox->getMargin();
	pBox->setMargin(0);
	btVector3 halfExtentsWithoutMargin = pBox->getHalfExtentsWithoutMargin();
	pBox->setMargin(margin);

	Vector3F halfExtents = PhysicsWorld::Convert(halfExtentsWithoutMargin);
	RenderObject* pDebugBox = StandardMesh::CreateCube8(4);
	VertexBuffer* pVertexBuffer = pDebugBox->GetMesh()->GetVertexBuffer();
	for (UInt i = 0; i < pVertexBuffer->GetQuantity(); i++)
	{
		Vector3F& rPosition = pVertexBuffer->Position3(i);
		rPosition.X() *= halfExtents.X();
		rPosition.Y() *= halfExtents.Y();
		rPosition.Z() *= halfExtents.Z();
		pVertexBuffer->Color4(i) = rColor;
	}

	pDebugBox->GetMesh()->UpdateModelBound();
	return pDebugBox;
}

//----------------------------------------------------------------------------
RenderObject* RigidBodyController::CreateDebugSphere(
	btSphereShape* pSphereShape, const Color32& rColor)
{
	RenderObject* pDebugSphere = StandardMesh::CreateSphere(10, 10,
		pSphereShape->getRadius(), 0, 4);

	VertexBuffer* pVertexBuffer = pDebugSphere->GetMesh()->GetVertexBuffer();
	for (UInt i = 0; i < pVertexBuffer->GetQuantity(); i++)
	{
		pVertexBuffer->Color4(i) = rColor;
	}

	return pDebugSphere;
}

//----------------------------------------------------------------------------
RenderObject* RigidBodyController::CreateDebugCapsule(
	btCapsuleShape* pCapsuleShape, const Color32& rColor)
{
	RenderObject* pDebugCapsule = StandardMesh::CreateCapsule(10, 10,
		pCapsuleShape->getRadius(), pCapsuleShape->getHalfHeight()*2, 0, 4);

	Int axis = pCapsuleShape->getUpAxis();
	Transformation transformation;
	Matrix3F rotation;
	VertexBuffer* pVertexBuffer = pDebugCapsule->GetMesh()->GetVertexBuffer();

	switch (axis)
	{
	case 0 :
		rotation.FromAxisAngle(Vector3F(0, 1, 0), MathF::HALF_PI);
		transformation.SetRotate(rotation);
		pVertexBuffer->ApplyForward(transformation, pVertexBuffer->GetData());
		break;
	case 1:
		rotation.FromAxisAngle(Vector3F(1, 0, 0), MathF::HALF_PI);
		transformation.SetRotate(rotation);
		pVertexBuffer->ApplyForward(transformation, pVertexBuffer->GetData());
		break;
	case 2:
		break;
	default:
		WIRE_ASSERT(false);
	}

	for (UInt i = 0; i < pVertexBuffer->GetQuantity(); i++)
	{
		pVertexBuffer->Color4(i) = rColor;
	}

	return pDebugCapsule;
}

//----------------------------------------------------------------------------
RenderObject* RigidBodyController::CreateDebugMesh(btStridingMeshInterface*
	pMeshInterface, const Color32& rColor)
{
	WIRE_ASSERT(pMeshInterface);

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(4);

	UChar* pVertexBase;
	Int vertexCount;
	PHY_ScalarType type;
	Int stride;
	UChar* pIndexBase;
	Int triangleStride;
	Int triangleCount;
	PHY_ScalarType indexType;

	pMeshInterface->getLockedVertexIndexBase(&pVertexBase,
		vertexCount, type, stride, &pIndexBase, triangleStride, triangleCount,
		indexType);

	WIRE_ASSERT(type == PHY_FLOAT);
	WIRE_ASSERT(triangleStride == 3*sizeof(UShort));
	WIRE_ASSERT(indexType == PHY_SHORT);
	if (type != PHY_FLOAT || triangleStride != 3*sizeof(UShort) ||
		indexType != PHY_SHORT)
	{
		return NULL;
	}

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexCount);
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(triangleCount*3);

	for (Int i = 0; i < vertexCount; i++)
	{
		Float* pVertexData = reinterpret_cast<Float*>(pVertexBase + i * stride);
		Vector3F v;
		v.X() = *pVertexData++;
		v.Y() = *pVertexData++;
		v.Z() = *pVertexData++;
		pVertexBuffer->Position3(i) = v;
		pVertexBuffer->Color4(i) = rColor;
	}

	UShort* pIndexData = reinterpret_cast<UShort*>(pIndexBase);
	for (Int i = 0; i < triangleCount*3; i += 3)
	{
		(*pIndexBuffer)[i] = pIndexData[i];
		(*pIndexBuffer)[i+1] = pIndexData[i+1];
		(*pIndexBuffer)[i+2] = pIndexData[i+2];
	}

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	RenderObject* pDebugMesh = WIRE_NEW RenderObject(pMesh);
	return pDebugMesh;
}
