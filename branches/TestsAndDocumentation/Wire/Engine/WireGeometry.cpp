// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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
	WIRE_ASSERT(pMesh);
	Init();
}

//----------------------------------------------------------------------------
Geometry::Geometry()
{
}

//----------------------------------------------------------------------------
Geometry::~Geometry()
{
}

//----------------------------------------------------------------------------
void Geometry::Init()
{
	System::Memset(States, 0, State::MAX_STATE_TYPE * sizeof(State*));
	StateSetID = System::MAX_UINT;
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

	// update light state
	Lights.RemoveAll();
	Lights.SetQuantity(pLightStack->GetQuantity());
	for (UInt i = 0; i < pLightStack->GetQuantity(); i++)
	{
		Lights[i] = (*pLightStack)[i];
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
		UInt id = pStateKeys->GetQuantity()+1;
		pStateKeys->Insert(key, id);
		StateSetID = id;
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
	UInt offset = 0;

	if (States[State::ALPHA])
	{
		StateAlpha* pState = StaticCast<StateAlpha>(States[State::ALPHA]);
		key = pState->ID;
	}

	offset = TInstanceID<StateAlpha>::GetMaxID()+1;
	if (States[State::CULL])
	{
		StateCull* pState = StaticCast<StateCull>(States[State::CULL]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateCull>::GetMaxID()+1;
	if (States[State::FOG])
	{
		StateFog* pState = StaticCast<StateFog>(States[State::FOG]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateFog>::GetMaxID()+1;
	if (States[State::MATERIAL])
	{
		StateMaterial* pState = StaticCast<StateMaterial>(
			States[State::MATERIAL]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateMaterial>::GetMaxID()+1;
	if (States[State::WIREFRAME])
	{
		StateWireframe* pState = StaticCast<StateWireframe>(States[
			State::WIREFRAME]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateWireframe>::GetMaxID()+1;
	if (States[State::ZBUFFER])
	{
		StateZBuffer* pState = StaticCast<StateZBuffer>(States[
			State::ZBUFFER]);
		key += pState->ID * offset;
	}

	for (UInt i = 0; i < Lights.GetQuantity(); i++)
	{
		offset *= (i == 0) ? TInstanceID<StateZBuffer>::GetMaxID()+1 :
			TInstanceID<Light>::GetMaxID()+1;
		key += Lights[i]->ID * offset;
	}

	WIRE_ASSERT(VerifyKey(key, offset));

	return key;
}

//----------------------------------------------------------------------------
Bool Geometry::VerifyKey(UInt key, UInt offset)
{
	for (Int i = (Lights.GetQuantity()-1); i >= 0 ; i--)
	{
		UInt id = key / offset;
		if (id != Lights[i]->ID)
		{
			WIRE_ASSERT(false /* Light state id calculation error */);
			return false;
		}

		key -= id * offset;
		offset /= (i == 0) ? TInstanceID<StateZBuffer>::GetMaxID()+1 :
			TInstanceID<Light>::GetMaxID()+1;
	}

	if (States[State::ZBUFFER])
	{
		StateZBuffer* pState = StaticCast<StateZBuffer>(States[
			State::ZBUFFER]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* ZBuffer state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateWireframe>::GetMaxID()+1;
	if (States[State::WIREFRAME])
	{
		StateWireframe* pState = StaticCast<StateWireframe>(States[
			State::WIREFRAME]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Wireframe state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateMaterial>::GetMaxID()+1;
	if (States[State::MATERIAL])
	{
		StateMaterial* pState = StaticCast<StateMaterial>(States[
			State::MATERIAL]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Material state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateFog>::GetMaxID()+1;
	if (States[State::FOG])
	{
		StateFog* pState = StaticCast<StateFog>(States[State::FOG]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Fog state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateCull>::GetMaxID()+1;
	if (States[State::CULL])
	{
		StateCull* pState = StaticCast<StateCull>(States[State::CULL]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Cull state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateAlpha>::GetMaxID()+1;
	if (States[State::ALPHA])
	{
		StateAlpha* pState = StaticCast<StateAlpha>(States[State::ALPHA]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Alpha state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	return true;
}

//----------------------------------------------------------------------------
void Geometry::MakeStatic(Bool forceStatic, Bool duplicateShared)
{
	VertexBuffer* pVertexBuffer = mspMesh->GetVertexBuffer();
	Bool isShared = pVertexBuffer->GetReferences() > 1 ||
		mspMesh->GetReferences() > 1;

	if (isShared && !duplicateShared)
	{
		return;
	}

	if (forceStatic)
	{
		WorldIsCurrent = true;
		WorldBoundIsCurrent = true;
	}

	if ((!(WorldIsCurrent && WorldBoundIsCurrent) || World.IsIdentity()))
	{
		return;
	}

	const VertexAttributes& rAttr = pVertexBuffer->GetAttributes();
	if (!rAttr.HasPosition() && !rAttr.HasNormal())
	{
		return;
	}

	// if the vertex buffer is shared, we duplicate it to apply the World
	// transformation
	if (isShared)
	{
		pVertexBuffer = WIRE_NEW VertexBuffer(mspMesh->GetVertexBuffer());
		mspMesh = WIRE_NEW Mesh(pVertexBuffer, mspMesh->GetIndexBuffer(),
			mspMesh->GetStartIndex(), mspMesh->GetIndexCount());
	}

	pVertexBuffer->ApplyForward(World, pVertexBuffer->GetData());

	World.MakeIdentity();
	mspMesh->UpdateModelBound();
	UpdateWorldBound();
}
