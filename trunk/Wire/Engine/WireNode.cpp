// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNode.h"

#include "WireCuller.h"
#include "WireCullerSorting.h"
#include "WireIndexBuffer.h"
#include "WireRenderer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Node, Spatial);

//----------------------------------------------------------------------------
Node::Node(UInt quantity, UInt growBy)
{
	Init(quantity, growBy);
}

//----------------------------------------------------------------------------
Node::Node(const Node* pNode)
	:
	Spatial(pNode)
{
	mEffects.SetQuantity(pNode->mEffects.GetQuantity());
	for (UInt i = 0; i < pNode->mEffects.GetQuantity(); i++)
	{
		mEffects[i] = pNode->mEffects[i];
	}	

	mspRenderObject = pNode->mspRenderObject;
	if (mspRenderObject)
	{
		mspRenderObjectWorldBound = BoundingVolume::Create();
		mspRenderObjectWorldBound->CopyFrom(pNode->mspRenderObjectWorldBound);
	}

	mChildren.SetMaxQuantity(pNode->mChildren.GetQuantity());
	for (UInt i = 0; i < pNode->mChildren.GetQuantity(); i++)
	{
		Node* pChild = DynamicCast<Node>(pNode->mChildren[i]);
		if (pChild)
		{
			AttachChild(WIRE_NEW Node(pChild));
		}
	}	

	Init(pNode->mChildren.GetMaxQuantity(), pNode->mChildren.GetGrowBy());
}

//----------------------------------------------------------------------------
Node::~Node()
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
 		DetachChildAt(i);
	}
}

//----------------------------------------------------------------------------
UInt Node::AttachChild(Spatial* pChild)
{
	// Some folks are under the impression that a node can have multiple
	// parents, the scene graph therefore being a DAG. That is not the case.
	// The parent-child relationships form a tree. This assertion is to let
	// folks know this and to warn them that a child is being kidnapped from
	// another parent. To be safe, you should really call DetachChild before
	// you reattach somewhere else with AttachChild or SetChild. If you do
	// call DetachChild first, be aware that the child might self-destruct.
	// If you want this not to happen, hang onto the child via a smart
	// pointer. For example,
	//
	//     Node* pNode0 = WIRE_NEW Node;
	//     Spatial* pChild0 = <...>;
	//     pNode0->AttachChild(pChild0);  // child at index 0
	//     Node* pNode1 = <...>;
	//
	//     // This asserts because pChild0 already has a parent (pNode0).
	//     pNode1->AttachChild(pChild0);
	//
	//     // Instead do this and avoid the potential self-destruction of
	//     // pChild0).
	//     SpatialPtr spSaveChild = pNode0->GetChild(0);
	//     pNode0->DetachChild(spSaveChild);
	//     pNode1->AttachChild(spSaveChild);

	WIRE_ASSERT(pChild && !pChild->GetParent());

	pChild->SetParent(this);

	// attach child in first available slot (if any)
	UInt quantity = GetQuantity();
	for (UInt i = 0; i < quantity; i++)
	{
		if (mChildren[i] == NULL)
		{
			mChildren[i] = pChild;
			return i;
		}
	}

	// all slots used, increase array size
	mChildren.Append(pChild);
	return quantity;
}

//----------------------------------------------------------------------------
Int Node::DetachChild(Spatial* pChild)
{
	if (pChild)
	{
		// search to see if child exists
		for (UInt i = 0; i < GetQuantity(); i++)
		{
			if (mChildren[i] == pChild)
			{
				// child found, detach it
				pChild->SetParent(NULL);
				mChildren[i] = NULL;
				return i;
			}
		}
	}

	return -1;
}

//----------------------------------------------------------------------------
SpatialPtr Node::DetachChildAt(UInt i)
{
	if (i < GetQuantity())
	{
		SpatialPtr spChild = mChildren[i];
		if (spChild)
		{
			// child exists in slot, detach it
			spChild->SetParent(NULL);
			mChildren[i] = NULL;
		}

		return spChild;
	}

	return NULL;
}

//----------------------------------------------------------------------------
SpatialPtr Node::SetChild(UInt i, Spatial* pChild)
{
	// Some folks are under the impression that a node can have multiple
	// parents, the scene graph therefore being a DAG. That is not the case.
	// The parent-child relationships form a tree. This assertion is to let
	// folks know this and to warn them that a child is being kidnapped from
	// another parent. To be safe, you should really call DetachChild before
	// you reattach somewhere else with AttachChild or SetChild. If you do
	// call DetachChild first, be aware that the child might self-destruct.
	// If you want this not to happen, hang onto the child via a smart
	// pointer. For example,
	//
	//     Node* pNode0 = WIRE_NEW Node;
	//     Spatial* pChild0 = <...>;
	//     pNode0->AttachChild(pChild0);  // child at index 0
	//     Node* pNode1 = <...>;
	//
	//     // This asserts because pChild0 already has a parent (pNode0).
	//     pNode1->AttachChild(pChild0);
	//
	//     // Instead do this and avoid the potential self-destruction of
	//     // pChild0).
	//     SpatialPtr spSaveChild = pNode0->GetChild(0);
	//     pNode0->DetachChild(spSaveChild);
	//     pNode1->AttachChild(spSaveChild);

	if (pChild)
	{
		WIRE_ASSERT(!pChild->GetParent());
	}

	if (i < GetQuantity())
	{
		// detach child currently in slot
		SpatialPtr spPreviousChild = mChildren[i];
		if (spPreviousChild)
		{
			spPreviousChild->SetParent(0);
		}

		// attach new child to slot
		if (pChild)
		{
			pChild->SetParent(this);
		}

		mChildren[i] = pChild;
		return spPreviousChild;
	}

	// index out of range, increase array size and attach new child
	pChild->SetParent(this);
	mChildren.Append(pChild);
	return NULL;
}

//----------------------------------------------------------------------------
SpatialPtr Node::GetChild(UInt i)
{
	if (i < GetQuantity())
	{
		return mChildren[i];
	}

	return NULL;
}

//----------------------------------------------------------------------------
void Node::UpdateWorldBound()
{
	Bool foundFirstBound = UpdateWorldBoundRenderObject();

	if (!WorldBoundIsCurrent)
	{
		for (UInt i = 0; i < GetQuantity(); i++)
		{
			Spatial* pChild = mChildren[i];
			if (pChild)
			{
				if (foundFirstBound)
				{
					// merge current world bound with child world bound
					WorldBound->GrowToContain(pChild->WorldBound);
				}
				else
				{
					// set world bound to first non-null child world bound
					foundFirstBound = true;
					WorldBound->CopyFrom(pChild->WorldBound);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void Node::UpdateWorldData(Double appTime, Bool updateControllers)
{
	Spatial::UpdateWorldData(appTime, updateControllers);

	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->UpdateGS(appTime, false, updateControllers);
		}
	}
}

//----------------------------------------------------------------------------
void Node::AttachEffect(Effect* pEffect)
{
	WIRE_ASSERT(pEffect);

	// Check if the effect is already in the list.
	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		if (mEffects[i] == pEffect)
		{
			// The effect already exists, so do nothing.
			return;
		}
	}

	// The effect is not in the current list, so add it.
	mEffects.Append(pEffect);
}

//----------------------------------------------------------------------------
void Node::UpdateState(States* pStates, Lights* pLights, Keys* pKeys)
{
	UpdateStateRenderObject(pStates, pLights, pKeys);

	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->UpdateRS(pStates, pLights, pKeys);
		}
	}
}

//----------------------------------------------------------------------------
void Node::GetVisibleSet(Culler& rCuller, Bool noCull)
{
	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		// This is a global effect. Place a 'begin' marker in the visible
		// set to indicate the effect is active.
		rCuller.Insert(mEffects[i], NULL);
	}

	GetVisibleSetRenderObject(rCuller, noCull);

	// All RenderObjects in the subtree are added to the visible set. If
	// a global effect is active, the RenderObjects in the subtree will be
	// drawn using it.
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		Spatial* pChild = mChildren[i];
		if (pChild)
		{
			pChild->OnGetVisibleSet(rCuller, noCull);
		}
	}

	for (UInt i = 0; i < mEffects.GetQuantity(); i++)
	{
		// Place an 'end' marker in the visible set to indicate that the
		// global effect is inactive.
		rCuller.Insert(NULL, NULL);
	}
}

//----------------------------------------------------------------------------
Spatial* Node::GetChildByName(const String& rName) const
{
	Spatial* pFound = NULL;
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		if (mChildren[i])
		{
			if (rName == mChildren[i]->GetName())
			{
				return mChildren[i];
			}

			const Node* pNode = DynamicCast<Node>(mChildren[i]);
			if (pNode)
			{
				pFound = pNode->GetChildByName(rName);
				if (pFound)
				{
					return pFound;
				}
			}
		}
	}

	return pFound;
}

//----------------------------------------------------------------------------
void Node::GetAllChildrenByName(const String& rName, TArray<Spatial*>&
	rChildren) const
{
	for (UInt i = 0; i < mChildren.GetQuantity(); i++)
	{
		if (mChildren[i])
		{
			if (rName == mChildren[i]->GetName())
			{
				rChildren.Append(mChildren[i]);
			}

			const Node* pNode = DynamicCast<Node>(mChildren[i]);
			if (pNode)
			{
				pNode->GetAllChildrenByName(rName, rChildren);
			}
		}
	}
}

//----------------------------------------------------------------------------
void Node::Bind(Renderer* pRenderer)
{
	if (pRenderer)
	{
		pRenderer->Bind(mspRenderObject);
	}

	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Spatial* pSpatial = GetChild(i);
		if (pSpatial)
		{
			pSpatial->Bind(pRenderer);
		}
	}
}

//----------------------------------------------------------------------------
void Node::Unbind(Renderer* pRenderer)
{
	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Spatial* pSpatial = GetChild(i);
		if (pSpatial)
		{
			pSpatial->Unbind(pRenderer);
		}
	}

	if (pRenderer)
	{
		pRenderer->Unbind(mspRenderObject);
	}
}

//----------------------------------------------------------------------------
void Node::WarmUpRendering(Renderer* pRenderer)
{
	WIRE_ASSERT(pRenderer);
	UpdateGS(0, true, false);

	Vector3F cameraLocation = WorldBound->GetCenter();
	cameraLocation.Z() += WorldBound->GetRadius();
	Vector3F viewDirection = -Vector3F::UNIT_Z;
	Vector3F up = Vector3F::UNIT_Y;
	Vector3F right = viewDirection.Cross(up);
	CameraPtr spCamera = WIRE_NEW Camera;
	spCamera->SetFrame(cameraLocation, viewDirection, up, right);

	Float fieldOfView = 60.0F;
	Float aspectRatio = 2;
	Float nearPlane = 0.1F;
	Float farPlane = WorldBound->GetRadius() * 2.0F;
	spCamera->SetFrustum(fieldOfView, aspectRatio, nearPlane, farPlane);

	CullerSorting culler;
	culler.SetCamera(spCamera);
	culler.ComputeVisibleSet(this);

	// TODO: how to deal with CULL_ALWAYS objects and LOD subgraphs that
	// have not been traversed?

	pRenderer->PreDraw(spCamera);
	pRenderer->Draw(culler.GetVisibleSets());
	pRenderer->PostDraw();
}

//----------------------------------------------------------------------------
void Node::PrepareForDynamicBatching(Bool forceWorldIsCurrent,
	Bool duplicateShared)
{
	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Node* pChild = DynamicCast<Node>(GetChild(i));
		if (pChild)
		{
			pChild->PrepareForDynamicBatching(forceWorldIsCurrent,
				duplicateShared);
		}
	}

	PrepareRenderObjectForDynamicBatching(forceWorldIsCurrent,
		duplicateShared);
}

//----------------------------------------------------------------------------
void Node::PrepareForStaticBatching(Bool forceWorldIsCurrent,
	Bool duplicateShared, TArray<MergeArray*>* pMergeArrays)
{
	Bool isInitiator = pMergeArrays == NULL;
	if (isInitiator)
	{
		PrepareForDynamicBatching(forceWorldIsCurrent, duplicateShared);
		pMergeArrays = WIRE_NEW TArray<MergeArray*>;
	}

	if (mspRenderObject && WorldIsCurrent && WorldBoundIsCurrent &&
		World.IsIdentity())
	{
		Mesh* pMesh = mspRenderObject->GetMesh();
		Bool isNewMesh = true;
		for (UInt i = 0; i < pMergeArrays->GetQuantity(); i++)
		{
			MergeArray* pMeshArray = (*pMergeArrays)[i];
			Mesh::VertexBuffers& rMVABs = (*pMeshArray)[0]->GetMesh()->
				GetVertexBuffers();
			Mesh::VertexBuffers& rMVBs = pMesh->GetVertexBuffers();

			Bool isIdentical = true;
			if (rMVABs.GetQuantity() == rMVBs.GetQuantity())
			{
				for (UInt j = 0; j < rMVBs.GetQuantity(); j++)
				{
					if (rMVBs[j]->GetAttributes().GetKey() !=
						rMVABs[j]->GetAttributes().GetKey())
					{
						isIdentical = false;
						break;
					}
				}
			}

			if (isIdentical)
			{
				pMeshArray->Append(mspRenderObject);
				isNewMesh = false;
				break;
			}
		}

		if (isNewMesh)
		{
			MergeArray* pNewArray = WIRE_NEW MergeArray;
			pNewArray->Append(mspRenderObject);
			pMergeArrays->Append(pNewArray);
		}
	}

	for (UInt i = 0; i < GetQuantity(); i++)
	{
		Node* pChild = DynamicCast<Node>(GetChild(i));
		if (pChild)
		{
			pChild->PrepareForStaticBatching(forceWorldIsCurrent,
				duplicateShared, pMergeArrays);
		}
	}

	if (isInitiator)
	{
		TArray<MergeArray*>* pSortedArrays = WIRE_NEW TArray<MergeArray*>;
		for (UInt i = 0; i < pMergeArrays->GetQuantity(); i++)
		{
			MergeArray* pMergeArray = (*pMergeArrays)[i];

			const UInt maxVtxQty = 0x10000;
			UInt vtxCount = 0;
			UInt idxCount = 0;
			MergeArray* pSortedArray = WIRE_NEW MergeArray(pMergeArray->
				GetQuantity(), 16);
			pSortedArrays->Append(pSortedArray);

			for (UInt i = 0; i < pMergeArray->GetQuantity(); i++)
			{
				Mesh* pMesh = (*pMergeArray)[i]->GetMesh();
				if ((vtxCount + pMesh->GetVertexCount()) < maxVtxQty)
				{
					vtxCount += pMesh->GetVertexCount();
					idxCount += pMesh->GetIndexCount();
				}
				else
				{
					pSortedArray = WIRE_NEW MergeArray(pMergeArray->
						GetQuantity(), 16);
					pSortedArrays->Append(pSortedArray);
					vtxCount = 0;
					idxCount = 0;
				}

				pSortedArray->Append((*pMergeArray)[i]);
			}

			WIRE_DELETE pMergeArray;
		}

		WIRE_DELETE pMergeArrays;
		pMergeArrays = NULL;

		for (UInt i = 0; i < pSortedArrays->GetQuantity(); i++)
		{
			MergeArray* pSortedArray = (*pSortedArrays)[i];
			MergeMeshes(pSortedArray);
			WIRE_DELETE pSortedArray;
		}

		WIRE_DELETE pSortedArrays;
		pSortedArrays = NULL;
	}
}

//----------------------------------------------------------------------------
void Node::MergeMeshes(MergeArray* pMergeArray)
{
	WIRE_ASSERT(pMergeArray->GetQuantity() > 0);
	if (pMergeArray->GetQuantity() == 1)
	{
		return;
	}

	TArray<Transformation*> transformations(pMergeArray->GetQuantity());
	TArray<UInt> keys(pMergeArray->GetQuantity());
	for (UInt i = 0; i < pMergeArray->GetQuantity(); i++)
	{
		UInt key = 0;
		enum
		{
			STATESET = 16,
			MATERIAL = 16,
		};

		WIRE_ASSERT((STATESET + MATERIAL) <= sizeof(key) * 8);

		RenderObject* pRenderObject = (*pMergeArray)[i];
		Material* pMaterial = pRenderObject->GetMaterial();
		if (pMaterial)
		{
			WIRE_ASSERT(pMaterial->ID < (1<<MATERIAL));
			key |= pMaterial->ID;
		}

		// If StateSetID is MAX_UINT, it wasn't initialized (call UpdateRS()
		// once or initialize manually)
		WIRE_ASSERT(pRenderObject->GetStateSetID() < (1<<STATESET));
		key |= pRenderObject->GetStateSetID() << (MATERIAL);

		keys.Append(key);
	}

	Object** pObjects = reinterpret_cast<Object**>(pMergeArray->GetArray());
 	CullerSorting::QuickSort(keys, pObjects, transformations.GetArray(), 0,
		pMergeArray->GetQuantity()-1);

	UInt vtxCount = 0;
	UInt idxCount = 0;
	Mesh* pMesh = NULL;
	for (UInt i = 0; i < pMergeArray->GetQuantity(); i++)
	{
		pMesh = (*pMergeArray)[i]->GetMesh();
		vtxCount += pMesh->GetVertexCount();
		idxCount += pMesh->GetIndexCount();
	}

	const UInt vbCount = pMesh->GetVertexBuffers().GetQuantity();
	Mesh::VertexBuffers vbs(vbCount);
	TArray<Float*> rawDst(vbCount);

	for (UInt j = 0; j < vbCount; j++)
	{
		VertexBuffer* pVB = WIRE_NEW VertexBuffer(pMesh->
			GetVertexBuffer(j)->GetAttributes(), vtxCount);
		vbs.Append(pVB);
		rawDst.Append(pVB->GetData());
	}

	IndexBuffer* pIB = WIRE_NEW IndexBuffer(idxCount);
	UShort* pRawIB = pIB->GetData();
	Int offset = 0;

	for (UInt i = 0; i < pMergeArray->GetQuantity(); i++)
	{
		pMesh = (*pMergeArray)[i]->GetMesh();
		for (UInt j = 0; j < vbCount; j++)
		{
			VertexBuffer* pMVB = pMesh->GetVertexBuffer(j);
			const UInt vtxCount = pMesh->GetVertexCount();
			pMVB->ApplyForward(Transformation::IDENTITY, rawDst[j], vtxCount,
				pMesh->GetMinIndex());
			const UInt vtxSize = pMVB->GetAttributes().GetVertexSize();
			rawDst[j] += (vtxSize / sizeof(Float)) * vtxCount;
		}

		IndexBuffer* pMIB = pMesh->GetIndexBuffer();
		offset -= pMesh->GetMinIndex();
		const UInt startIdx = pMesh->GetStartIndex();
		for (UInt j = startIdx; j < (startIdx + pMesh->GetIndexCount()); j++)
		{
			WIRE_ASSERT((((*pMIB)[j])+offset) < 0x10000);
			*pRawIB++ = static_cast<UShort>((((*pMIB)[j])+offset));
		}

		offset += pMesh->GetVertexCount();
	}

	UInt startIndex = 0;
	for (UInt i = 0; i < pMergeArray->GetQuantity(); i++)
	{
		pMesh = (*pMergeArray)[i]->GetMesh();
		const UInt indexCount = pMesh->GetIndexCount();
		Mesh* pMergedMesh = WIRE_NEW Mesh(vbs, pIB, startIndex, indexCount);
		startIndex += indexCount;
		(*pMergeArray)[i]->SetMesh(pMergedMesh);
	}
}

//----------------------------------------------------------------------------
void Node::Init(UInt quantity, UInt growBy)
{
	mChildren.SetMaxQuantity(quantity);
	mChildren.SetGrowBy(growBy);
}

//----------------------------------------------------------------------------
Node::Node(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer, Material* pMaterial,
	UInt quantity, UInt growBy)
{
	Init(quantity, growBy);

	Mesh* pMesh = WIRE_NEW Mesh(pVBuffer, pIBuffer);
	mspRenderObject = WIRE_NEW RenderObject(pMesh, pMaterial);
	InitRenderObject();
}

//----------------------------------------------------------------------------
Node::Node(Mesh* pMesh, Material* pMaterial, UInt quantity, UInt growBy)
{
	Init(quantity, growBy);

	mspRenderObject = WIRE_NEW RenderObject(pMesh, pMaterial);
	InitRenderObject();
}

//----------------------------------------------------------------------------
Node::Node(RenderObject* pRenderObject, UInt quantity, UInt growBy)
{
	Init(quantity, growBy);

	mspRenderObject = pRenderObject;
	InitRenderObject();
}

//----------------------------------------------------------------------------
Bool Node::UpdateWorldBoundRenderObject()
{
	if (mspRenderObject)
	{
		mspRenderObject->GetMesh()->GetModelBound()->TransformBy(World,
			mspRenderObjectWorldBound);

		if (!WorldBoundIsCurrent)
		{
			WorldBound->CopyFrom(mspRenderObjectWorldBound);
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------
void Node::UpdateStateRenderObject(States* pStates, Lights* pLights,
	Keys* pKeys)
{
	if (!mspRenderObject)
	{
		return;
	}

	// update global state
	StatePtr* rStates = mspRenderObject->GetStates();
	for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
	{
		TArray<State*>& rState = pStates[i];
		rStates[i] = rState[rState.GetQuantity()-1];
	}

	// update light state
	TArray<LightPtr>* pRenderObjectLights = mspRenderObject->GetLights();
	if (!pRenderObjectLights)
	{
		pRenderObjectLights = WIRE_NEW TArray<LightPtr>;
	}

	pRenderObjectLights->RemoveAll();
	pRenderObjectLights->SetQuantity(pLights->GetQuantity());
	for (UInt i = 0; i < pLights->GetQuantity(); i++)
	{
		(*pRenderObjectLights)[i] = (*pLights)[i];
	}

	// check if other RenderObjects share the same states
	UInt key = GetStateSetKey();
	UInt* pStateSetID = pKeys->Find(key);
	if (pStateSetID)
	{
		mspRenderObject->SetStateSetID(*pStateSetID);
	}
	else
	{
		UInt id = pKeys->GetQuantity()+1;
		pKeys->Insert(key, id);
		mspRenderObject->SetStateSetID(id);
	}
}

//----------------------------------------------------------------------------
void Node::GetVisibleSetRenderObject(Culler& rCuller, Bool noCull)
{
	// Add this RenderObject if it's not culled. (Its bounding volume is
	// smaller or equal the bounding volume of this node).
	if (mspRenderObject)
	{
		if (GetQuantity() == 0)
		{
			const Vector3F& rCamPos = rCuller.GetCamera()->GetLocation();
			Vector3F pos = mspRenderObjectWorldBound->GetCenter() - rCamPos;

			rCuller.Insert(mspRenderObject, &World, pos);
		}
		else
		{
			UInt savePlaneState = rCuller.GetPlaneState();
			if (noCull || rCuller.IsVisible(mspRenderObjectWorldBound, true))
			{
				const Vector3F& rCamPos = rCuller.GetCamera()->GetLocation();
				Vector3F pos = mspRenderObjectWorldBound->GetCenter() - rCamPos;

				rCuller.Insert(mspRenderObject, &World, pos);
			}

			rCuller.SetPlaneState(savePlaneState);
		}
	}
}

//----------------------------------------------------------------------------
UInt Node::GetStateSetKey()
{
	WIRE_ASSERT(mspRenderObject);
	UInt key = 0;
	UInt offset = 0;

	StatePtr* rStates = mspRenderObject->GetStates();
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

	TArray<LightPtr>* pLights = mspRenderObject->GetLights();
	for (UInt i = 0; pLights && (i < pLights->GetQuantity()); i++)
	{
		offset *= (i == 0) ? TInstanceID<StateZBuffer>::GetMaxID()+1 :
			TInstanceID<Light>::GetMaxID()+1;
	WIRE_ASSERT((*pLights)[i]);
	key += (*pLights)[i]->ID * offset;
	}

	WIRE_ASSERT(VerifyKey(key, offset));

	return key;
}

//----------------------------------------------------------------------------
Bool Node::VerifyKey(UInt key, UInt offset)
{
	WIRE_ASSERT(mspRenderObject);
	TArray<LightPtr>* pLights = mspRenderObject->GetLights();
	if (pLights)
	{
		for (Int i = (pLights->GetQuantity()-1); i >= 0; i--)
		{
			UInt id = key / offset;
			if (id != (*pLights)[i]->ID)
			{
				WIRE_ASSERT(false /* Light state id calculation error */);
				return false;
			}

			key -= id * offset;
			offset /= (i == 0) ? TInstanceID<StateZBuffer>::GetMaxID()+1 :
				TInstanceID<Light>::GetMaxID()+1;
		}
	}

	StatePtr* rStates = mspRenderObject->GetStates();
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
void Node::InitRenderObject()
{
	if (!mspRenderObject)
	{
		return;
	}

	mspRenderObjectWorldBound = BoundingVolume::Create();
	if (!mspRenderObject->GetLights())
	{
		mspRenderObject->SetLights(WIRE_NEW TArray<LightPtr>);
	}
}

//----------------------------------------------------------------------------
void Node::PrepareRenderObjectForDynamicBatching(Bool forceWorldIsCurrent,
	Bool duplicateShared)
{
	if (!mspRenderObject)
	{
		return;
	}

	Mesh* pMesh = mspRenderObject->GetMesh();
	WIRE_ASSERT(pMesh);
	VertexBuffer* const pPositions = pMesh->GetPositionBuffer();
	WIRE_ASSERT(pPositions);
	VertexBuffer* const pNormals = pMesh->GetNormalBuffer();

	Bool isRenderObjectShared = mspRenderObject->GetReferences() > 1;
	Bool isMeshShared = pMesh->GetReferences() > 1;
	Bool isPositionShared = pPositions->GetReferences() > 1;
	Bool isNormalShared = pNormals && pNormals->GetReferences() > 1;
	Bool isShared = isMeshShared ||isPositionShared || isNormalShared ||
		isRenderObjectShared;

	if (isShared && !duplicateShared)
	{
		return;
	}

	if (forceWorldIsCurrent)
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
		TArray<VertexBufferPtr> vertexBuffers;
		for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
		{
			VertexBuffer* pVertexBuffer = pMesh->GetVertexBuffer(i);
			if (pVertexBuffer == pPositions)
			{
				if (isRenderObjectShared || isMeshShared || isPositionShared)
				{
					pVertexBuffer = WIRE_NEW VertexBuffer(pVertexBuffer);		
				}
			}
			else if (pVertexBuffer == pNormals)
			{
				if (isRenderObjectShared || isMeshShared || isNormalShared)
				{
					pVertexBuffer = WIRE_NEW VertexBuffer(pVertexBuffer);
				}
			}

			vertexBuffers.Append(pVertexBuffer);
		}

		if (isRenderObjectShared)
		{
			mspRenderObject = WIRE_NEW RenderObject(mspRenderObject);
		}

		pMesh = WIRE_NEW Mesh(vertexBuffers, pMesh->GetIndexBuffer(),
			pMesh->GetStartIndex(), pMesh->GetIndexCount());
		mspRenderObject->SetMesh(pMesh);
	}

	VertexBuffer* const pUniquePositions = pMesh->GetPositionBuffer();
	pUniquePositions->ApplyForward(World, pUniquePositions->GetData());
	VertexBuffer* const pUniqueNormals = pMesh->GetNormalBuffer();
	if (pUniqueNormals && (pUniquePositions != pUniqueNormals))
	{
		pUniqueNormals->ApplyForward(World, pUniqueNormals->GetData());
	}

	World.MakeIdentity();
	pMesh->GetModelBound()->CopyFrom(mspRenderObjectWorldBound);
}
