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
#include "WireMesh.h"
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
	mspMaterial(pMaterial)
{
	mspMesh = WIRE_NEW Mesh(pVBuffer, pIBuffer);
	Init();
}

//----------------------------------------------------------------------------
Geometry::Geometry(Mesh* pMesh, Material* pMaterial)
	:
	mspMesh(pMesh),
	mspMaterial(pMaterial)
{
	Init();
}

//----------------------------------------------------------------------------
Geometry::~Geometry()
{
}

//----------------------------------------------------------------------------
void Geometry::Init()
{
	System::Memset(States, 0, State::MAX_STATE_TYPE * sizeof(State*));
}

//----------------------------------------------------------------------------
void Geometry::UpdateWorldBound()
{
	mspMesh->GetModelBound()->TransformBy(World, WorldBound);
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
		StateSetID = pStateKeys->GetQuantity()+1;
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
