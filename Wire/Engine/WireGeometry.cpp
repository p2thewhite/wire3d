// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGeometry.h"
#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Geometry, Spatial);

//----------------------------------------------------------------------------
Geometry::Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer, Material*
	pMaterial)
 	:
 	mspVBuffer(pVBuffer),
	mspIBuffer(pIBuffer),
	mspModelBound(BoundingVolume::Create()),
	mspMaterial(pMaterial)
{
	System::Memset(States, 0, State::MAX_STATE_TYPE * sizeof(State*));
	UpdateModelBound();
}

//----------------------------------------------------------------------------
Geometry::~Geometry()
{
}

//----------------------------------------------------------------------------
void Geometry::UpdateModelBound()
{
	mspModelBound->ComputeFromData(mspVBuffer);
}

//----------------------------------------------------------------------------
void Geometry::UpdateWorldBound()
{
	mspModelBound->TransformBy(World, WorldBound);
}

//----------------------------------------------------------------------------
void Geometry::UpdateState(TArray<State*>* pGStack,
	TArray<Light*>* pLStack)
{
	// update global state
	for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
	{
		TArray<State*>& rState = pGStack[i];
  		States[i] = rState[rState.GetQuantity()-1];
	}

	// update light state
	Lights.RemoveAll();
	Lights.SetQuantity(pLStack->GetQuantity());
	for (UInt i = 0; i < pLStack->GetQuantity(); i++)
	{
		Lights[i] = (*pLStack)[i];
	}
}

//----------------------------------------------------------------------------
void Geometry::GetVisibleSet(Culler& rCuller, Bool)
{
	rCuller.Insert(this, NULL);
}

//----------------------------------------------------------------------------
void Geometry::GenerateNormals(Bool ignoreHardEdges)
{
	if (!mspVBuffer || !mspIBuffer)
	{
		return;
	}

	if (mspVBuffer->GetAttributes().GetNormalChannels() != 3)
	{
		return;
	}

	UInt* const pIndices = mspIBuffer->GetData();

	// collect the triangles each vertex is part of
	TArray<TArray<UInt> > buckets(mspVBuffer->GetVertexQuantity());
	buckets.SetQuantity(mspVBuffer->GetVertexQuantity());

	UInt triIndex = 0;
	for (UInt i = 0; i < mspIBuffer->GetIndexQuantity(); i += 3)
	{
		buckets[pIndices[i]].Append(triIndex);
		buckets[pIndices[i+1]].Append(triIndex);
		buckets[pIndices[i+2]].Append(triIndex);
		triIndex++;
	}

	if (ignoreHardEdges)
	{
		for (UInt j = 0; j < mspVBuffer->GetVertexQuantity(); j++)
		{
			const Vector3F& vertex = mspVBuffer->Position3(j);
			for (UInt i = j+1; i < mspVBuffer->GetVertexQuantity(); i++)
			{
				if (vertex == mspVBuffer->Position3(i))
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
	TArray<Vector3F> faceNormals(mspIBuffer->GetIndexQuantity()/3);
	for (UInt i = 0; i < mspIBuffer->GetIndexQuantity(); i +=3)
	{
		Vector3F v1 = mspVBuffer->Position3(pIndices[i+1]) -
			mspVBuffer->Position3(pIndices[i]);
		Vector3F v2 = mspVBuffer->Position3(pIndices[i+2]) -
			mspVBuffer->Position3(pIndices[i+1]);

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

		mspVBuffer->Normal3(i) = normal;
	}
}
