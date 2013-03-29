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
	mpRigidBody(pRigidBody),
	mIsStaticCollider(true)
{
	if (mpRigidBody)
	{
		btVector3 gravity = mpRigidBody->getGravity();
		if (gravity.length2() > 0)
		{
			mIsStaticCollider = false;
		}
	}
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

	if (mpRigidBody->getMotionState())
	{
		Matrix3F mat;
		btTransform trans;
		Vector3F pos;
		QuaternionF quat;

		if (!mIsStaticCollider || mspDebugShape)
		{
			mpRigidBody->getMotionState()->getWorldTransform(trans);
			pos = PhysicsWorld::Convert(trans.getOrigin());
			quat = PhysicsWorld::Convert(trans.getRotation());
			quat.ToRotationMatrix(mat);

			if (!mIsStaticCollider)
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
	WIRE_ASSERT(stride == 4);
	WIRE_ASSERT(triangleStride == 6);
	WIRE_ASSERT(indexType == PHY_SHORT);
	if (type != PHY_FLOAT || stride != 4 || triangleStride != 6 ||
		indexType != PHY_SHORT)
	{
		return NULL;
	}

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attributes,
		vertexCount);
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(triangleCount*3);

	Float* pVertexData = reinterpret_cast<Float*>(pVertexBase);
	for (Int i = 0; i < vertexCount; i++)
	{
		pVertexBuffer->Position3(i) = Vector3F(pVertexData[i*3],
			pVertexData[i*3+1], pVertexData[i*3+2]);
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