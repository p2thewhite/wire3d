#include "CollisionShapeToGeometryConverter.h"
#include "WireVertexAttributes.h"
#include "WireVertexBuffer.h"
#include "WireIndexBuffer.h"
#include "WireStateWireframe.h"
#include "BulletUtils.h"

//----------------------------------------------------------------------------
CollisionShapeToGeometryConverter::CollisionShapeToGeometryConverter()
{
}

//----------------------------------------------------------------------------
CollisionShapeToGeometryConverter::~CollisionShapeToGeometryConverter()
{
}

//----------------------------------------------------------------------------
Geometry* CollisionShapeToGeometryConverter::Convert(btCollisionShape* pShape, const Color32& rColor)
{
	if (pShape->getShapeType() == BOX_SHAPE_PROXYTYPE)
	{
		return CreateWireframeBox(static_cast<btBoxShape*>(pShape), rColor);
	}
	else if (pShape->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
	{
		return CreateWireframeSphere(static_cast<btSphereShape*>(pShape), rColor);
	}
	else if (pShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
	{
		return CreateWireframeMesh(static_cast<btBvhTriangleMeshShape*>(pShape), rColor);
	}

	WIRE_ASSERT(false /* Unsupported collision shape */);

	return NULL;
}

//----------------------------------------------------------------------------
Geometry* CollisionShapeToGeometryConverter::CreateWireframeBox(btBoxShape* pBoxShape, const Color32& rColor)
{
	btVector3 scaling = pBoxShape->getLocalScaling();
	float margin = pBoxShape->getMargin();

	// Reseting scaling and margin to acquire the real half extents
	pBoxShape->setMargin(0);
	pBoxShape->setLocalScaling(btVector3(1, 1, 1));

	// Acquiring the real half extents
	btVector3 halfExtentsWithoutMargin = pBoxShape->getHalfExtentsWithoutMargin();

	// Restoring previous scaling and margin values
	pBoxShape->setLocalScaling(scaling);
	pBoxShape->setMargin(margin);

	Vector3F halfExtents = BulletUtils::Convert(halfExtentsWithoutMargin);

	// Creating the box

	const Vector3F vertices[] =
	{
		// side 1
		Vector3F(-halfExtents.X(),  halfExtents.Y(),  halfExtents.Z()),
		Vector3F( halfExtents.X(),  halfExtents.Y(),  halfExtents.Z()),
		Vector3F( halfExtents.X(), -halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(), -halfExtents.Y(),  halfExtents.Z()),
		// side 2
		Vector3F( halfExtents.X(),  halfExtents.Y(), -halfExtents.Z()),
		Vector3F( halfExtents.X(),  halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(),  halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(),  halfExtents.Y(), -halfExtents.Z()),
		// side 3
		Vector3F( halfExtents.X(), -halfExtents.Y(),  halfExtents.Z()),
		Vector3F( halfExtents.X(),  halfExtents.Y(),  halfExtents.Z()),
		Vector3F( halfExtents.X(),  halfExtents.Y(), -halfExtents.Z()),
		Vector3F( halfExtents.X(), -halfExtents.Y(), -halfExtents.Z()),
		// side 4
		Vector3F(-halfExtents.X(),  halfExtents.Y(), -halfExtents.Z()),
		Vector3F( halfExtents.X(),  halfExtents.Y(), -halfExtents.Z()),
		Vector3F( halfExtents.X(), -halfExtents.Y(), -halfExtents.Z()),
		Vector3F(-halfExtents.X(), -halfExtents.Y(), -halfExtents.Z()),
		// side 5
		Vector3F( halfExtents.X(), -halfExtents.Y(), -halfExtents.Z()),
		Vector3F( halfExtents.X(), -halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(), -halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(), -halfExtents.Y(), -halfExtents.Z()),
		// side 6
		Vector3F(-halfExtents.X(), -halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(),  halfExtents.Y(),  halfExtents.Z()),
		Vector3F(-halfExtents.X(),  halfExtents.Y(), -halfExtents.Z()),
		Vector3F(-halfExtents.X(), -halfExtents.Y(), -halfExtents.Z())
	};

	const UInt indexes[] =
	{
		// side 1
		0, 1, 2,
		0, 2, 3,
		// side 2
		4, 5, 6,
		4, 6, 7,
		// side 3
		8, 9, 10,
		8, 10, 11,
		// side 4
		12, 14, 13,
		12, 15, 14,
		// side 5
		16, 18, 17,
		16, 19, 18,
		// side 6
		20, 22, 21,
		20, 23, 22
	};

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);

	UInt vertexQuantity = sizeof(vertices) / sizeof(Vector3F);

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attributes, vertexQuantity);
	WIRE_ASSERT(pVertexBuffer);

	for (UInt i = 0; i < pVertexBuffer->GetQuantity(); i++)
	{
		pVertexBuffer->Position3(i) = vertices[i];
		pVertexBuffer->Color3(i) = rColor;
	}

	UInt indexQuantity = sizeof(indexes) / sizeof(UInt);
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(indexQuantity);
	for	(UInt i = 0; i < indexQuantity; i++)
	{
		(*pIndexBuffer)[i] = indexes[i];
	}

	Geometry* pBox = WIRE_NEW Geometry(pVertexBuffer, pIndexBuffer);
	WIRE_ASSERT(pBox);

	// Set wireframe rendering state
	StateWireframe* pWireframe = WIRE_NEW StateWireframe();
	pWireframe->Enabled = true;

	pBox->AttachState(pWireframe);

	return pBox;
}

//----------------------------------------------------------------------------
Geometry* CollisionShapeToGeometryConverter::CreateWireframeSphere(btSphereShape* pSphereShape, const Color32& rColor)
{
	btVector3 scaling = pSphereShape->getLocalScaling();
	float margin = pSphereShape->getMargin();

	// Reseting scaling and margin to acquire the real half extents
	pSphereShape->setMargin(0);
	pSphereShape->setLocalScaling(btVector3(1, 1, 1));

	// TODO: Define as parameters
	UInt rings = 10;
	UInt sectors = 10;

	// Restoring previous scaling and margin values
	pSphereShape->setLocalScaling(scaling);
	pSphereShape->setMargin(margin);

	// Creating the sphere

	Float R = 1.0f / (Float) (rings - 1);
	Float S = 1.0f / (Float) (sectors - 1);

	TArray<Vector3F> vertices;
	for (UInt r = 0; r < rings; r++) {
		for (UInt s = 0; s < sectors; s++) {
			Float y = sin(-MathF::HALF_PI + MathF::PI * r * R);
			Float x = cos(2 * MathF::PI * s * S) * sin(MathF::PI * r * R);
			Float z = sin(2 * MathF::PI * s * S) * sin(MathF::PI * r * R);
			vertices.Append(Vector3F(x, y, z));
		}
	}

	TArray<UInt> indexes;
	for (UInt r = 0; r < rings; r++) {
		for (UInt s = 0; s < sectors; s++) {
			indexes.Append(r * sectors + s);
			indexes.Append(r * sectors + (s + 1));
			indexes.Append((r + 1) * sectors + (s + 1));
			indexes.Append((r + 1) * sectors + s);
		}
	}

	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetColorChannels(3);

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attributes, vertices.GetQuantity());
	WIRE_ASSERT(pVertexBuffer);

	for (UInt i = 0; i < pVertexBuffer->GetQuantity(); i++)
	{
		pVertexBuffer->Position3(i) = vertices[i];
		pVertexBuffer->Color3(i) = rColor;
	}

	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(indexes.GetQuantity());
	for	(UInt i = 0; i < pIndexBuffer->GetQuantity(); i++)
	{
		(*pIndexBuffer)[i] = indexes[i];
	}

	Geometry* pSphere = WIRE_NEW Geometry(pVertexBuffer, pIndexBuffer);
	WIRE_ASSERT(pSphere);

	// Set wireframe rendering state
	StateWireframe* pWireframe = WIRE_NEW StateWireframe();
	pWireframe->Enabled = true;

	pSphere->AttachState(pWireframe);

	return pSphere;
}

//----------------------------------------------------------------------------
Geometry* CollisionShapeToGeometryConverter::CreateWireframeMesh(
	btBvhTriangleMeshShape* pTriangleMeshShape, const Color32& rColor)
{
	Mesh* pMesh = BulletUtils::Convert(pTriangleMeshShape);

	Geometry* pGeometry = WIRE_NEW Geometry(pMesh);

	pGeometry->GetMesh()->GenerateNormals();

	// Set wireframe rendering state
	StateWireframe* pWireframe = WIRE_NEW StateWireframe();
	pWireframe->Enabled = true;

	pGeometry->AttachState(pWireframe);

	return pGeometry;
}
