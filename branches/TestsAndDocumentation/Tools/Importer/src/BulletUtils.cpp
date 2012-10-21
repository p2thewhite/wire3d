#include "BulletUtils.h"

#include "WireVertexBuffer.h"
#include "WireIndexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
BulletUtils::BulletUtils()
{
}

//----------------------------------------------------------------------------
BulletUtils::~BulletUtils()
{
}

//----------------------------------------------------------------------------
btTriangleIndexVertexArray* BulletUtils::Convert(Mesh* pMesh)
{
	unsigned short int* pTriangleIndexBase;
	UShort* pIndexBufferData;
	Float* pVertexBufferData;
	btScalar* pVertexBase;

	VertexBuffer* pVertexBuffer = pMesh->GetVertexBuffer();
	IndexBuffer* pIndexBuffer = pMesh->GetIndexBuffer();

	// Copying index buffer data

	pIndexBufferData = pIndexBuffer->GetData();

	UInt size = pIndexBuffer->GetQuantity();
	pTriangleIndexBase = WIRE_NEW unsigned short int[size];
	for (UInt i = 0; i < size; i++)
	{
		pTriangleIndexBase[i] = pIndexBufferData[i];
	}

	// Copying vertex buffer data

	pVertexBufferData = pVertexBuffer->GetData();

	size = pVertexBuffer->GetQuantity();
	pVertexBase = WIRE_NEW btScalar[size];
	for (UInt i = 0; i < size; i++)
	{
		pVertexBase[i] = pVertexBufferData[i];
	}

	btTriangleIndexVertexArray* pTriangleIndexVertexArray =
		WIRE_NEW btTriangleIndexVertexArray();

	// Creating bullet mesh description struct

	btIndexedMesh indexedMesh;

	indexedMesh.m_numTriangles = pIndexBuffer->GetQuantity() / 3;
	indexedMesh.m_triangleIndexBase = (const unsigned char*)pTriangleIndexBase;
	indexedMesh.m_triangleIndexStride = sizeof(unsigned short int) * 3;
	indexedMesh.m_numVertices = pVertexBuffer->GetQuantity();
	indexedMesh.m_vertexBase = (const unsigned char*)pVertexBase;
	indexedMesh.m_vertexStride = sizeof(btScalar);

	pTriangleIndexVertexArray->addIndexedMesh(indexedMesh, PHY_SHORT);

	return pTriangleIndexVertexArray;
}

//----------------------------------------------------------------------------
Mesh* BulletUtils::Convert(btTriangleMeshShape* pTriangleMeshShape)
{
	VertexAttributes attributes;
	attributes.SetPositionChannels(3);
	attributes.SetNormalChannels(3);

	unsigned char* pVertexBase;
	int numberOfVertices;
	PHY_ScalarType type;
	int stride;
	unsigned char* pIndexBase;
	int indexStride;
	int numberOfTriangles;
	PHY_ScalarType indicesType;

	pTriangleMeshShape->getMeshInterface()->getLockedVertexIndexBase(
		&pVertexBase, numberOfVertices, type, stride, &pIndexBase,
		indexStride, numberOfTriangles, indicesType);

	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attributes,
		numberOfVertices);
	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(numberOfTriangles * 3);

	for (Int i = 0; i < numberOfVertices; i += 3)
	{
		float* pVertexData = (float*)pVertexBase;
		pVertexBuffer->Position3(i) = Vector3F(
			pVertexData[i], pVertexData[i + 1], pVertexData[i + 2]);
	}

	for (Int i = 0; i < numberOfTriangles; i++)
	{
		(*pIndexBuffer)[i] = pIndexBase[i];
		(*pIndexBuffer)[i + 1] = pIndexBase[i + 1];
		(*pIndexBuffer)[i + 2] = pIndexBase[i + 2];
	}

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	pMesh->GenerateNormals();

	return pMesh;
}
