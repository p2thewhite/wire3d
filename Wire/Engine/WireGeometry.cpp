// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGeometry.h"

#include "WireBoundingVolume.h"
#include "WireCuller.h"
#include "WireMesh.h"
#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Geometry, Spatial);

//----------------------------------------------------------------------------
Geometry::Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer, Material*
	pMaterial)
{
	Mesh* pMesh = WIRE_NEW Mesh(pVBuffer, pIBuffer);
	Init(pMesh, pMaterial);
}

//----------------------------------------------------------------------------
Geometry::Geometry(Mesh* pMesh, Material* pMaterial)
{
	Init(pMesh, pMaterial);
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
void Geometry::Init(Mesh* pMesh, Material* pMaterial)
{
	mspRenderObject = WIRE_NEW RenderObject(pMesh, pMaterial);
	mspRenderObject->WorldBound = WorldBound;
}

//----------------------------------------------------------------------------
void Geometry::UpdateWorldBound()
{
	GetMesh()->GetModelBound()->TransformBy(World, WorldBound);
}

//----------------------------------------------------------------------------
void Geometry::UpdateWorldData(Double appTime)
{
	Spatial::UpdateWorldData(appTime);
	WIRE_ASSERT(mspRenderObject);
	mspRenderObject->World = World;
}

//----------------------------------------------------------------------------
void Geometry::UpdateState(TArray<State*>* pStateStacks,
	TArray<Light*>* pLightStack, THashTable<UInt, UInt>* pStateKeys)
{
	// update global state
	StatePtr* rStates = GetStates();
	for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
	{
		TArray<State*>& rState = pStateStacks[i];
  		rStates[i] = rState[rState.GetQuantity()-1];
	}

	// update light state
	TArray<LightPtr>& rLights = GetLights();
	rLights.RemoveAll();
	rLights.SetQuantity(pLightStack->GetQuantity());
	for (UInt i = 0; i < pLightStack->GetQuantity(); i++)
	{
		rLights[i] = (*pLightStack)[i];
	}

	// check if other Geometry objects share the same states
	UInt key = GetStateSetKey();
	UInt* pStateSetID = pStateKeys->Find(key);
	if (pStateSetID)
	{
		mspRenderObject->SetStateSetID(*pStateSetID);
	}
	else
	{
		UInt id = pStateKeys->GetQuantity()+1;
		pStateKeys->Insert(key, id);
		mspRenderObject->SetStateSetID(id);
	}
}

//----------------------------------------------------------------------------
void Geometry::GetVisibleSet(Culler& rCuller, Bool)
{
	WIRE_ASSERT(mspRenderObject);
	rCuller.Insert(mspRenderObject);
}

//----------------------------------------------------------------------------
UInt Geometry::GetStateSetKey()
{
	UInt key = 0;
	UInt offset = 0;

	StatePtr* rStates = GetStates();
	if (rStates[State::ALPHA])
	{
		StateAlpha* pState = StaticCast<StateAlpha>(rStates[State::ALPHA]);
		key = pState->ID;
	}

	offset = TInstanceID<StateAlpha>::GetMaxID()+1;
	if (rStates[State::CULL])
	{
		StateCull* pState = StaticCast<StateCull>(rStates[State::CULL]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateCull>::GetMaxID()+1;
	if (rStates[State::FOG])
	{
		StateFog* pState = StaticCast<StateFog>(rStates[State::FOG]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateFog>::GetMaxID()+1;
	if (rStates[State::MATERIAL])
	{
		StateMaterial* pState = StaticCast<StateMaterial>(rStates[
			State::MATERIAL]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateMaterial>::GetMaxID()+1;
	if (rStates[State::WIREFRAME])
	{
		StateWireframe* pState = StaticCast<StateWireframe>(rStates[
			State::WIREFRAME]);
		key += pState->ID * offset;
	}

	offset *= TInstanceID<StateWireframe>::GetMaxID()+1;
	if (rStates[State::ZBUFFER])
	{
		StateZBuffer* pState = StaticCast<StateZBuffer>(rStates[
			State::ZBUFFER]);
		key += pState->ID * offset;
	}

	TArray<LightPtr>& rLights = GetLights();
	for (UInt i = 0; i < rLights.GetQuantity(); i++)
	{
		offset *= (i == 0) ? TInstanceID<StateZBuffer>::GetMaxID()+1 :
			TInstanceID<Light>::GetMaxID()+1;
		WIRE_ASSERT(rLights[i]);
		key += rLights[i]->ID * offset;
	}

	WIRE_ASSERT(VerifyKey(key, offset));

	return key;
}

//----------------------------------------------------------------------------
Bool Geometry::VerifyKey(UInt key, UInt offset)
{
	TArray<LightPtr>& rLights = GetLights();
	for (Int i = (rLights.GetQuantity()-1); i >= 0 ; i--)
	{
		UInt id = key / offset;
		if (id != rLights[i]->ID)
		{
			WIRE_ASSERT(false /* Light state id calculation error */);
			return false;
		}

		key -= id * offset;
		offset /= (i == 0) ? TInstanceID<StateZBuffer>::GetMaxID()+1 :
			TInstanceID<Light>::GetMaxID()+1;
	}

	StatePtr* rStates = GetStates();
	if (rStates[State::ZBUFFER])
	{
		StateZBuffer* pState = StaticCast<StateZBuffer>(rStates[
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
	if (rStates[State::WIREFRAME])
	{
		StateWireframe* pState = StaticCast<StateWireframe>(rStates[
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
	if (rStates[State::MATERIAL])
	{
		StateMaterial* pState = StaticCast<StateMaterial>(rStates[
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
	if (rStates[State::FOG])
	{
		StateFog* pState = StaticCast<StateFog>(rStates[State::FOG]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Fog state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateCull>::GetMaxID()+1;
	if (rStates[State::CULL])
	{
		StateCull* pState = StaticCast<StateCull>(rStates[State::CULL]);
		UInt id = key / offset;
		if (id != pState->ID)
		{
			WIRE_ASSERT(false /* Cull state id calculation error */);
			return false;
		}

		key -= id * offset;
	}

	offset /= TInstanceID<StateAlpha>::GetMaxID()+1;
	if (rStates[State::ALPHA])
	{
		StateAlpha* pState = StaticCast<StateAlpha>(rStates[State::ALPHA]);
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
void Geometry::Bind(Renderer* pRenderer)
{
	if (pRenderer)
	{
		pRenderer->Bind(mspRenderObject);
	}
}

//----------------------------------------------------------------------------
void Geometry::Unbind(Renderer* pRenderer)
{
	if (pRenderer)
	{
		pRenderer->Unbind(mspRenderObject);
	}
}

//----------------------------------------------------------------------------
void Geometry::MakeStatic(Bool forceStatic, Bool duplicateShared)
{
	VertexBuffer* const pPositions = GetMesh()->GetPositionBuffer();
	WIRE_ASSERT(pPositions);
	VertexBuffer* const pNormals = GetMesh()->GetNormalBuffer();

	Bool isMeshShared = GetMesh()->GetReferences() > 1;
	Bool isPositionShared = pPositions->GetReferences() > 1;
	Bool isNormalShared = pNormals && pNormals->GetReferences() > 1;
	Bool isShared = isMeshShared ||isPositionShared || isNormalShared;

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

	// if the mesh or a vertex buffer containing positions or normals is
	// shared, we duplicate it to apply the World transformation
	if (isShared)
	{
		TArray<VertexBuffer*> vertexBuffers;
		for (UInt i = 0; i < GetMesh()->GetVertexBuffers().GetQuantity(); i++)
		{
			VertexBuffer* pVertexBuffer = GetMesh()->GetVertexBuffer(i);
			if (pVertexBuffer == pPositions)
			{
				if (isMeshShared || isPositionShared)
				{
					pVertexBuffer = WIRE_NEW VertexBuffer(pVertexBuffer);		
				}
			}
			else if (pVertexBuffer == pNormals)
			{
				if (isMeshShared || isNormalShared)
				{
					pVertexBuffer = WIRE_NEW VertexBuffer(pVertexBuffer);
				}
			}

			vertexBuffers.Append(pVertexBuffer);
		}

		SetMesh(WIRE_NEW Mesh(vertexBuffers, GetMesh()->GetIndexBuffer(),
			GetMesh()->GetStartIndex(), GetMesh()->GetIndexCount()));
	}

	VertexBuffer* const pUniquePositions = GetMesh()->GetPositionBuffer();
	pUniquePositions->ApplyForward(World, pUniquePositions->GetData());
	VertexBuffer* const pUniqueNormals = GetMesh()->GetNormalBuffer();
	if (pUniqueNormals && (pUniquePositions != pUniqueNormals))
	{
		pUniqueNormals->ApplyForward(World, pUniqueNormals->GetData());
	}

	World.MakeIdentity();
	mspRenderObject->World.MakeIdentity();
	GetMesh()->UpdateModelBound();
	UpdateWorldBound();
}
