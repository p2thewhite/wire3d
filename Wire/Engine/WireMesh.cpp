// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireMesh.h"

#include "WireBoundingVolume.h"
#include "WireIndexBuffer.h"
#include "WireVertexBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Mesh, Object);

//----------------------------------------------------------------------------
Mesh::Mesh(VertexBuffer* pVertexBuffer, IndexBuffer* pIndexBuffer)
	:
	mspVertexBuffer(pVertexBuffer),
	mspIndexBuffer(pIndexBuffer),
	mspModelBound(BoundingVolume::Create())
{
	UpdateModelBound();
}

//----------------------------------------------------------------------------
void Mesh::UpdateModelBound()
{
	mspModelBound->ComputeFromData(mspVertexBuffer);
}

//----------------------------------------------------------------------------
void Mesh::GenerateNormals(Bool ignoreHardEdges)
{
	if (!mspVertexBuffer || !mspIndexBuffer)
	{
		return;
	}

	if (mspVertexBuffer->GetAttributes().GetNormalChannels() != 3)
	{
		return;
	}

	UInt* const pIndices = mspIndexBuffer->GetData();

	// collect the triangles each vertex is part of
	TArray<TArray<UInt> > buckets(mspVertexBuffer->GetQuantity());
	buckets.SetQuantity(mspVertexBuffer->GetQuantity());

	UInt triIndex = 0;
	for (UInt i = 0; i < mspIndexBuffer->GetQuantity(); i += 3)
	{
		buckets[pIndices[i]].Append(triIndex);
		buckets[pIndices[i+1]].Append(triIndex);
		buckets[pIndices[i+2]].Append(triIndex);
		triIndex++;
	}

	if (ignoreHardEdges)
	{
		for (UInt j = 0; j < mspVertexBuffer->GetQuantity(); j++)
		{
			const Vector3F& vertex = mspVertexBuffer->Position3(j);
			for (UInt i = j+1; i < mspVertexBuffer->GetQuantity(); i++)
			{
				if (vertex == mspVertexBuffer->Position3(i))
				{
					UInt origCount = buckets[j].GetQuantity();
					for (UInt k = 0; k < buckets[i].GetQuantity(); k++)
					{
						buckets[j].Append(buckets[i][k]);
					}

					for (UInt k = 0; k < origCount; k++)
					{
						buckets[i].Append(buckets[j][k]);
					}
				}
			}
		}
	}

	// calculate the normals of the individual triangles
	TArray<Vector3F> faceNormals(mspIndexBuffer->GetQuantity()/3);
	for (UInt i = 0; i < mspIndexBuffer->GetQuantity(); i +=3)
	{
		Vector3F v1 = mspVertexBuffer->Position3(pIndices[i+1]) -
			mspVertexBuffer->Position3(pIndices[i]);
		Vector3F v2 = mspVertexBuffer->Position3(pIndices[i+2]) -
			mspVertexBuffer->Position3(pIndices[i+1]);

		Vector3F normal = v1.Cross(v2);
		normal.Normalize();
		faceNormals.Append(normal);
	}

	// calculate the normal of the vertex from the normals of its faces
	for (UInt i = 0; i < buckets.GetQuantity(); i++)
	{
		Vector3F normal(Vector3F::ZERO);
		for (UInt j = 0; j < buckets[i].GetQuantity(); j++)
		{
			normal += faceNormals[buckets[i][j]];
		}

		if (buckets[i].GetQuantity() > 0)
		{
			normal /= static_cast<Float>(buckets[i].GetQuantity());
			normal.Normalize();
		}
		else
		{
			// vertex not used in mesh, use a default normal
			normal = Vector3F::UNIT_X;
		}

		mspVertexBuffer->Normal3(i) = normal;
	}
}
