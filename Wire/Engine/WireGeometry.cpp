// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGeometry.h"

#include "WireBoundingVolume.h"
#include "WireCuller.h"
#include "WireEffect.h"
#include "WireIndexBuffer.h"
#include "WireLight.h"
#include "WireMaterial.h"
#include "WireStateAlpha.h"
#include "WireStateCull.h"
#include "WireStateFog.h"
#include "WireStateMaterial.h"
#include "WireStateWireframe.h"
#include "WireStateZBuffer.h"
#include "WireVertexBuffer.h"

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
void Geometry::UpdateState(TArray<State*>* pStateStacks,
	TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys)
{
	// update global state
	for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
	{
		TArray<State*>& rState = pStateStacks[i];
  		States[i] = rState[rState.GetQuantity()-1];
	}

	// check if other Geometry objects share the same states
	UInt key = GetStateSetKey();
	UInt* pStateSetID = pStateKeys->Find(key);
	if (pStateSetID)
	{
		StateSetID = *pStateSetID;
	}
	else
	{
		pStateKeys->Insert(key, pStateKeys->GetQuantity()+1);
	}

	// update light state
	Lights.RemoveAll();
	Lights.SetQuantity(pLightStack->GetQuantity());
	for (UInt i = 0; i < pLightStack->GetQuantity(); i++)
	{
		Lights[i] = (*pLightStack)[i];
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
	TArray<TArray<UInt> > buckets(mspVBuffer->GetQuantity());
	buckets.SetQuantity(mspVBuffer->GetQuantity());

	UInt triIndex = 0;
	for (UInt i = 0; i < mspIBuffer->GetQuantity(); i += 3)
	{
		buckets[pIndices[i]].Append(triIndex);
		buckets[pIndices[i+1]].Append(triIndex);
		buckets[pIndices[i+2]].Append(triIndex);
		triIndex++;
	}

	if (ignoreHardEdges)
	{
		for (UInt j = 0; j < mspVBuffer->GetQuantity(); j++)
		{
			const Vector3F& vertex = mspVBuffer->Position3(j);
			for (UInt i = j+1; i < mspVBuffer->GetQuantity(); i++)
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
	TArray<Vector3F> faceNormals(mspIBuffer->GetQuantity()/3);
	for (UInt i = 0; i < mspIBuffer->GetQuantity(); i +=3)
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

//----------------------------------------------------------------------------
UInt Geometry::GetStateSetKey()
{
	UInt key = 0;

	// number of bits we use for each state's ID
	enum
	{
		ALPHA = 7,		// 2^7-1 = 127 Alpha states can be handled
		CULL = 2,		// 2^2-1 = 3
		FOG = 4,		// 2^4-1 = 15
		MATERIAL = 12,	// 2^12-1 = 4095
		WIREFRAME = 2,	// 2^2-1 = 3
		ZBUFFER = 5		// 2^5-1 = 31
	};

	WIRE_ASSERT((ALPHA + CULL + FOG + MATERIAL + WIREFRAME + ZBUFFER) <=
		sizeof(key) * 8); // The sum of the ranges must fit in the key

	// The following asserts let you know when you have created more states
	// than the key can handle. This is only important if you need the
	// StateSetID to be unique (e.g. the CullerSorting class uses it to
	// sort its objects by state), otherwise you can ignore the asserts
	// completely.
	if (States[State::ALPHA])
	{
		StateAlpha* pState = StaticCast<StateAlpha>(States[State::ALPHA]);
		WIRE_ASSERT(pState->ID < (1<<ALPHA));
		key |= pState->ID;
	}

	if (States[State::CULL])
	{
		StateCull* pState = StaticCast<StateCull>(States[State::CULL]);
		WIRE_ASSERT(pState->ID < (1<<CULL));
		key |= pState->ID << ALPHA;
	}

	if (States[State::FOG])
	{
		StateFog* pState = StaticCast<StateFog>(States[State::FOG]);
		WIRE_ASSERT(pState->ID < (1<<FOG));
		key |= pState->ID << (ALPHA+CULL);
	}

	if (States[State::MATERIAL])
	{
		StateMaterial* pState = StaticCast<StateMaterial>(
			States[State::MATERIAL]);
		WIRE_ASSERT(pState->ID < (1<<MATERIAL));
		key |= pState->ID << (ALPHA+CULL+FOG);
	}

	if (States[State::WIREFRAME])
	{
		StateWireframe* pState = StaticCast<StateWireframe>(
			States[State::WIREFRAME]);
		WIRE_ASSERT(pState->ID < (1<<WIREFRAME));
		key |= pState->ID << (ALPHA+CULL+FOG+MATERIAL);
	}

	if (States[State::ZBUFFER])
	{
		StateZBuffer* pState = StaticCast<StateZBuffer>(
			States[State::ZBUFFER]);
		WIRE_ASSERT(pState->ID < (1<<ZBUFFER));
		key |= pState->ID << (ALPHA+CULL+FOG+MATERIAL+WIREFRAME);
	}

	return key;
}
