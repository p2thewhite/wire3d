// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireImage2D.h"
#include "WireIndexBuffer.h"
#include "WireLight.h"
#include "WireMesh.h"
#include "WireNode.h"
#include "WireTexture2D.h"
#include "WireVertexBuffer.h"
#include "WireVisibleSet.h"

#ifdef WIRE_WII
#include "WireGXIndexBuffer.h"
#include "WireGXTexture2D.h"
#include "WireGXVertexBuffer.h"
#else
#include "WireDx9IndexBuffer.h"
#include "WireDx9Texture2D.h"
#include "WireDx9VertexBuffer.h"
#endif

using namespace Wire;

Renderer* Renderer::s_pRenderer = NULL;

//----------------------------------------------------------------------------
void Renderer::Initialize(UInt width, UInt height)
{
	mWidth = width;
	mHeight = height;
	mMaxAnisotropy = 1.0F;

	mBatchedIndexBuffer = NULL;
	mBatchedVertexBuffer = NULL;
	mDynamicBatchingThreshold = 0;

	ResetStatistics();
	mStatistics.VBOCount = 0;
	mStatistics.VBOTotalSize = 0;
	mStatistics.IBOCount = 0;
	mStatistics.IBOTotalSize = 0;
	mStatistics.TextureCount = 0;
	mStatistics.TextureTotalSize = 0;

	s_pRenderer = this;
}

//----------------------------------------------------------------------------
void Renderer::Terminate()
{
	ReleaseResources();
	mspCamera = NULL;

	DestroyAll(mIndexBufferMap);
	DestroyAll(mVertexBufferMap);
	DestroyAll(mTexture2DMap);
	DestroyBatchingBuffers();

	s_pRenderer = NULL;
}

//----------------------------------------------------------------------------
void Renderer::ReleaseResources()
{
	if (mspIndexBuffer)
	{
		Disable(mspIndexBuffer);
	}

	if (mspVertexBuffer)
	{
		Disable(mspVertexBuffer);
	}

	if (mspMaterial)
	{
		Disable(mspMaterial);
	}

	for (UInt i = 0; i < mTexture2Ds.GetQuantity(); i++)
	{
		if (mTexture2Ds[i])
		{
			Disable(mTexture2Ds[i], i);
		}
	}

	for (UInt i = 0; i < mLights.GetQuantity(); i++)
	{
		if (mLights[i])
		{
			SetLight(NULL, i);
		}
	}

	for (UInt i = 0; i < State::MAX_STATE_TYPE; i++)
	{
		mspStates[i] = NULL;
	}
}

//----------------------------------------------------------------------------
void Renderer::BindAll(const Spatial* pSpatial)
{
	if (!pSpatial || !s_pRenderer)
	{
		return;
	}

	const Node* pNode = DynamicCast<Node>(pSpatial);
	if (pNode)
	{
		for (UInt i = 0; i < pNode->GetQuantity(); i++)
		{
			BindAll(const_cast<Node*>(pNode)->GetChild(i));
		}
	}

	const Geometry* pGeometry = DynamicCast<Geometry>(pSpatial);
	if (pGeometry)
	{
		const Mesh* pMesh = pGeometry->GetMesh();
		if (pMesh)
		{
			s_pRenderer->Bind(pMesh->GetIndexBuffer());
			s_pRenderer->Bind(pMesh->GetVertexBuffer());
		}

		const Material* pMaterial = pGeometry->GetMaterial();
		if (pMaterial)
		{
			for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
			{
				s_pRenderer->Bind(pMaterial->GetTexture(i));
			}
		}
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const Spatial* pSpatial)
{
	if (!pSpatial || !s_pRenderer)
	{
		return;
	}

	const Node* pNode = DynamicCast<Node>(pSpatial);
	if (pNode)
	{
		for (UInt i = 0; i < pNode->GetQuantity(); i++)
		{
			UnbindAll(const_cast<Node*>(pNode)->GetChild(i));
		}
	}

	const Geometry* pGeometry = DynamicCast<Geometry>(pSpatial);
	if (pGeometry)
	{
		const Mesh* pMesh = pGeometry->GetMesh();
		if (pMesh)
		{
			s_pRenderer->Unbind(pMesh->GetIndexBuffer());
			s_pRenderer->Unbind(pMesh->GetVertexBuffer());
		}

		const Material* pMaterial = pGeometry->GetMaterial();
		if (pMaterial)
		{
			for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
			{
				s_pRenderer->Unbind(pMaterial->GetTexture(i));
			}
		}
	}
}

//----------------------------------------------------------------------------
PdrIndexBuffer* Renderer::Bind(const IndexBuffer* pIndexBuffer)
{
	WIRE_ASSERT(pIndexBuffer);
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);

	if (!pValue)
	{
		PdrIndexBuffer* pPdrIndexBuffer = WIRE_NEW PdrIndexBuffer(this,
			pIndexBuffer);
		mIndexBufferMap.Insert(pIndexBuffer, pPdrIndexBuffer);
		mStatistics.IBOCount++;
		mStatistics.IBOTotalSize += pPdrIndexBuffer->GetBufferSize();
		return pPdrIndexBuffer;
	}

	return *pValue;
}

//----------------------------------------------------------------------------
void Renderer::Unbind(const IndexBuffer* pIndexBuffer)
{
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);

	if (pValue)
	{
		mStatistics.IBOCount--;
		mStatistics.IBOTotalSize -= (*pValue)->GetBufferSize();
		WIRE_DELETE *pValue;
		mIndexBufferMap.Remove(pIndexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const IndexBuffer* pIndexBuffer)
{
	if (s_pRenderer)
	{
		s_pRenderer->Unbind(pIndexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::Enable(const IndexBuffer* pIndexBuffer)
{
	WIRE_ASSERT(mspIndexBuffer == NULL /* Disable the previous IB first. */);
	WIRE_ASSERT(pIndexBuffer);
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);
	if (pValue)
	{
		(*pValue)->Enable(this);
	}
	else
	{
		PdrIndexBuffer* pPdrIndexBuffer = Bind(pIndexBuffer);
		pPdrIndexBuffer->Enable(this);
	}

	mspIndexBuffer = const_cast<IndexBuffer*>(pIndexBuffer);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const IndexBuffer* pIndexBuffer)
{
	WIRE_ASSERT(pIndexBuffer);
	WIRE_ASSERT(mspIndexBuffer == pIndexBuffer /* This IB is not enabled */);
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);
	if (pValue)
	{
		(*pValue)->Disable(this);
	}
	else
	{
		WIRE_ASSERT(false); // Index buffer is not bound
	}

	mspIndexBuffer = NULL;
}

//----------------------------------------------------------------------------
void Renderer::Set(const IndexBuffer* pIndexBuffer)
{
	if (mspIndexBuffer != pIndexBuffer)
	{
		if (mspIndexBuffer)
		{
			Disable(mspIndexBuffer);
		}

		Enable(pIndexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::Update(const IndexBuffer* pIndexBuffer)
{
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);
	if (pValue)
	{
		(*pValue)->Update(pIndexBuffer);
	}
	else
	{
		WIRE_ASSERT(false); // Index buffer is not bound
	}
}

//----------------------------------------------------------------------------
PdrIndexBuffer* Renderer::GetResource(const IndexBuffer* pIndexBuffer)
{
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);

	if (pValue)
	{
		return *pValue;
	}

	return NULL;
}

//----------------------------------------------------------------------------
PdrVertexBuffer* Renderer::Bind(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(pVertexBuffer);
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);

	if (!pValue)
	{
		PdrVertexBuffer* pPdrVertexBuffer = WIRE_NEW PdrVertexBuffer(this,
			pVertexBuffer);
		mVertexBufferMap.Insert(pVertexBuffer, pPdrVertexBuffer);
		mStatistics.VBOCount++;
		mStatistics.VBOTotalSize += pPdrVertexBuffer->GetBufferSize();
		return pPdrVertexBuffer;
	}

	return *pValue;
}

//----------------------------------------------------------------------------
void Renderer::Unbind(const VertexBuffer* pVertexBuffer)
{
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);

	if (pValue)
	{
		mStatistics.VBOCount--;
		mStatistics.VBOTotalSize -= (*pValue)->GetBufferSize();
		WIRE_DELETE *pValue;
		mVertexBufferMap.Remove(pVertexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const VertexBuffer* pVertexBuffer)
{
	if (s_pRenderer)
	{
		s_pRenderer->Unbind(pVertexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::Enable(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(mspVertexBuffer == NULL /* Disable the previous VB first. */);
	WIRE_ASSERT(pVertexBuffer);
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);
	if (pValue)
	{
		(*pValue)->Enable(this);
	}
	else
	{
		PdrVertexBuffer* pPdrVertexBuffer =	Bind(pVertexBuffer);
		pPdrVertexBuffer->Enable(this);
	}

	mspVertexBuffer = const_cast<VertexBuffer*>(pVertexBuffer);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(pVertexBuffer);
	WIRE_ASSERT(mspVertexBuffer == pVertexBuffer /* This VB is not enabled */);
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);
	if (pValue)
	{
		(*pValue)->Disable(this);
	}
	else
	{
		WIRE_ASSERT(false); // Vertex buffer is not bound
	}

	mspVertexBuffer = NULL;
}

//----------------------------------------------------------------------------
void Renderer::Set(const VertexBuffer* pVertexBuffer)
{
	if (mspVertexBuffer != pVertexBuffer)
	{
		if (mspVertexBuffer)
		{
			Disable(mspVertexBuffer);
		}

		Enable(pVertexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::Update(const VertexBuffer* pVertexBuffer)
{
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);
	if (pValue)
	{
		(*pValue)->Update(pVertexBuffer);
	}
	else
	{
		WIRE_ASSERT(false); // Vertex buffer is not bound
	}
}

//----------------------------------------------------------------------------
PdrVertexBuffer* Renderer::GetResource(const VertexBuffer* pVertexBuffer)
{
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);
	if (pValue)
	{
		return *pValue;
	}

	return NULL;
}

//----------------------------------------------------------------------------
PdrTexture2D* Renderer::Bind(const Texture2D* pTexture)
{
	WIRE_ASSERT(pTexture);
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);

	if (!pValue)
	{
		PdrTexture2D* pPdrTexture = WIRE_NEW PdrTexture2D(this, pTexture);
		mTexture2DMap.Insert(pTexture, pPdrTexture);
		mStatistics.TextureCount++;
		mStatistics.TextureTotalSize += pPdrTexture->GetBufferSize();
		return pPdrTexture;
	}

	return *pValue;
}

//----------------------------------------------------------------------------
void Renderer::Unbind(const Texture2D* pTexture)
{
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);

	if (pValue)
	{
		mStatistics.TextureCount--;
		mStatistics.TextureTotalSize -= (*pValue)->GetBufferSize();
		WIRE_DELETE *pValue;
		mTexture2DMap.Remove(pTexture);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const Texture2D* pTexture)
{
	if (s_pRenderer)
	{
		s_pRenderer->Unbind(pTexture);
	}
}

//----------------------------------------------------------------------------
void Renderer::Enable(const Texture2D* pTexture, UInt unit)
{
	WIRE_ASSERT(mTexture2Ds.GetQuantity() > unit);
	WIRE_ASSERT(mTexture2Ds[unit] == NULL /* Disable previous first */);
	WIRE_ASSERT(pTexture);
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);
	if (pValue)
	{
		(*pValue)->Enable(this, pTexture, unit);
	}
	else
	{
		PdrTexture2D* pPdrTexture =	Bind(pTexture);
		pPdrTexture->Enable(this, pTexture, unit);
	}

	mTexture2Ds[unit] = const_cast<Texture2D*>(pTexture);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const Texture2D* pTexture, UInt unit)
{
	WIRE_ASSERT(mTexture2Ds.GetQuantity() > unit);
	WIRE_ASSERT(mTexture2Ds[unit] == pTexture /* This Tex is not enabled */);
	WIRE_ASSERT(pTexture);
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);
	if (pValue)
	{
		(*pValue)->Disable(this, unit);
	}
	else
	{
		WIRE_ASSERT(false); // Texture is not bound
	}

	mTexture2Ds[unit] = NULL;
}

//----------------------------------------------------------------------------
void Renderer::Set(const Texture2D* pTexture, UInt unit)
{
	WIRE_ASSERT(mTexture2Ds.GetQuantity() > unit);
	if (mTexture2Ds[unit] != pTexture)
	{
		if (mTexture2Ds[unit])
		{
			Disable(mTexture2Ds[unit], unit);
		}

		Enable(pTexture, unit);
	}
}

//----------------------------------------------------------------------------
PdrTexture2D* Renderer::GetResource(const Texture2D* pTexture)
{
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);

	if (pValue)
	{
		return *pValue;
	}

	return NULL;
}

//----------------------------------------------------------------------------
void Renderer::DestroyAll(IndexBufferMap& rIndexBufferMap)
{
	while (rIndexBufferMap.GetQuantity() > 0)
	{
		IndexBufferMap::Iterator it(&rIndexBufferMap);
		const IndexBuffer* pKey;
		it.GetFirst(&pKey);
		Unbind(pKey);
	}

	WIRE_ASSERT(rIndexBufferMap.GetQuantity() == 0);
}

//----------------------------------------------------------------------------
void Renderer::DestroyAll(VertexBufferMap& rVertexBufferMap)
{
	while (rVertexBufferMap.GetQuantity() > 0)
	{
		VertexBufferMap::Iterator it(&rVertexBufferMap);
		const VertexBuffer* pKey;
		it.GetFirst(&pKey);
		Unbind(pKey);
	}

	WIRE_ASSERT(rVertexBufferMap.GetQuantity() == 0);
}

//----------------------------------------------------------------------------
void Renderer::DestroyAll(Texture2DMap& rTexture2DMap)
{
	while (rTexture2DMap.GetQuantity() > 0)
	{
		Texture2DMap::Iterator it(&rTexture2DMap);
		const Texture2D* pKey;
		it.GetFirst(&pKey);
		Unbind(pKey);
	}

	WIRE_ASSERT(rTexture2DMap.GetQuantity() == 0);
}

//----------------------------------------------------------------------------
void Renderer::Enable(const Material* pMaterial)
{
	if (!pMaterial)
	{
		return;
	}

	for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
	{
		Enable(pMaterial->GetTexture(i), i);
		SetBlendMode(pMaterial->GetBlendMode(i), i, pMaterial->GetTexture(i)->
			GetImage()->HasAlpha());
	}

	mspMaterial = const_cast<Material*>(pMaterial);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const Material* pMaterial)
{
	if (!pMaterial)
	{
		return;
	}

	for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
	{
		Disable(pMaterial->GetTexture(i), i);
	}

	mspMaterial = NULL;
}

//----------------------------------------------------------------------------
void Renderer::Set(const Material* pMaterial)
{
	if (mspMaterial != pMaterial)
	{
		if (mspMaterial)
		{
			Disable(mspMaterial);
		}

		Enable(pMaterial);
	}
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pGeometry, Bool restoreState, Bool useEffect)
{
	WIRE_ASSERT(pGeometry && pGeometry->GetMesh());
	if (useEffect && pGeometry->GetEffectQuantity() > 0)
	{
		for (UInt i = 0; i < pGeometry->GetEffectQuantity(); i++)
		{
			VisibleObject visibleObject;
			visibleObject.Object = pGeometry;
			visibleObject.GlobalEffect = NULL;
			pGeometry->GetEffect(i)->Draw(this, pGeometry, 0, 0,
				&visibleObject, restoreState);
		}

		return;
	}

	Mesh* pMesh = pGeometry->GetMesh();
	Bool usesNormals = pMesh->GetVertexBuffer()->GetAttributes().HasNormal();
	if (restoreState)
	{
		Enable(pGeometry->States);
		Enable(pGeometry->Lights);
		Enable(pMesh->GetIndexBuffer());
		Enable(pMesh->GetVertexBuffer());
		Enable(pGeometry->GetMaterial());

		SetWorldTransformation(pGeometry->World, usesNormals);
		DrawElements(pGeometry->ActiveIndexCount, pGeometry->StartIndex);

		Disable(pGeometry->GetMaterial());
		Disable(pMesh->GetVertexBuffer());
		Disable(pMesh->GetIndexBuffer());
		Disable(pGeometry->Lights);
		Disable(pGeometry->States);
	}
	else
	{
		Set(pGeometry->States);
		Set(pGeometry->Lights);
		Set(pMesh->GetIndexBuffer());
		Set(pMesh->GetVertexBuffer());
		Set(pGeometry->GetMaterial());

		SetWorldTransformation(pGeometry->World, usesNormals);
		DrawElements(pGeometry->ActiveIndexCount, pGeometry->StartIndex);
	}
}

//----------------------------------------------------------------------------
void Renderer::DrawScene(VisibleSet* pVisibleSet)
{
	// NOTE: The stack of 2-tuples is limited to having MAX_GLOBAL_EFFECTS
	// elements. This should be plenty, because the chances of having that
	// many global effects in the same path is small (that is a *lot* of
	// effects to apply in one frame). If it needs to be larger for your
	// applications, increase the maximum size.
	UInt indexStack[MAX_GLOBAL_EFFECTS][2]; // startIndex, finalIndex
	UInt top = 0;							// stack is initially empty
	indexStack[0][0] = 0;
	indexStack[0][1] = 0;

	const UInt visibleQuantity = pVisibleSet->GetQuantity();
	VisibleObject* pVisible = pVisibleSet->GetVisible();
	for (UInt i = 0; i < visibleQuantity; i++)
	{
		if (pVisible[i].Object)
		{
			if (pVisible[i].GlobalEffect)
			{
				Draw(pVisible, indexStack[0][0], indexStack[0][1]);

				// Begin the scope of a global effect.
				top++;
				WIRE_ASSERT(top < static_cast<Int>(MAX_GLOBAL_EFFECTS));
				indexStack[top][0] = i;
				indexStack[top][1] = i;
			}
			else
			{
				// Found a leaf Geometry object.
				indexStack[top][1]++;
			}
		}
		else
		{
			// End the scope of a global effect.
			WIRE_ASSERT(!pVisible[i].GlobalEffect);
			UInt min = indexStack[top][0];
			UInt max = indexStack[top][1];

			pVisible[min].GlobalEffect->Draw(this, pVisible[min].Object,
				min+1, max, pVisible, false);

			if (--top > 0)
			{
				indexStack[top][1] = max + 1;
			}
			else
			{
				indexStack[0][0] = max + 2;
				indexStack[0][1] = max + 2;
			}
		}
	}

	Draw(pVisible, indexStack[0][0], indexStack[0][1]);

	ReleaseResources();
}

//----------------------------------------------------------------------------
void Renderer::DrawScene(TArray<VisibleSet*>& rVisibleSets)
{
	for (UInt i = 0; i < rVisibleSets.GetQuantity(); i++)
	{
		DrawScene(rVisibleSets[i]);
	}
}

//----------------------------------------------------------------------------
void Renderer::Draw(VisibleObject* const pVisible, UInt min, UInt max)
{
	if (min == max)
	{
		return;
	}

	if (mDynamicBatchingThreshold > 0)
	{
		while (min < max)
		{
			UInt idx = min;
			for (; idx < max-1; idx++)
			{
				WIRE_ASSERT(DynamicCast<Geometry>(pVisible[idx].Object));
				Geometry* pA = StaticCast<Geometry>(pVisible[idx].Object);
				Material* pMA = pA->GetMaterial();
				UInt mA = pMA ? pMA->ID : 0;
				WIRE_ASSERT(DynamicCast<Geometry>(pVisible[idx+1].Object));
				Geometry* pB = StaticCast<Geometry>(pVisible[idx+1].Object);
				Material* pMB = pB->GetMaterial();
				UInt mB = pMB ? pMB->ID : 0;

				if (mA != mB)
				{
					break;
				}

				UInt vA = pA->GetMesh()->GetVertexBuffer()->GetAttributes().
					GetKey();
				UInt vB = pB->GetMesh()->GetVertexBuffer()->GetAttributes().
					GetKey();

				if ((vA != vB) || (pA->StateSetID != pB->StateSetID))
				{
					break;
				}
			}

			if (idx > min)
			{
//				BatchAndDraw(pVisible, min, idx+1);

				for (UInt i = min; i < idx+1; i++)
				{
					WIRE_ASSERT(DynamicCast<Geometry>(pVisible[i].Object));
					Draw(StaticCast<Geometry>(pVisible[i].Object), false, true);
				}
			}
			else
			{
				WIRE_ASSERT(DynamicCast<Geometry>(pVisible[min].Object));
				Draw(StaticCast<Geometry>(pVisible[min].Object), false, true);
			}

			min = idx+1;
		}
	}
	else
	{
		for (UInt i = min; i < max; i++)
		{
			WIRE_ASSERT(DynamicCast<Geometry>(pVisible[i].Object));
			Draw(StaticCast<Geometry>(pVisible[i].Object), false, true);
		}
	}
}

//----------------------------------------------------------------------------
void Renderer::BatchAndDraw(VisibleObject* const /*pVisible*/, UInt /*min*/, UInt /*max*/)
{
// 	PdrIndexBuffer* const pIBPdr = mBatchedIndexBuffer;
// 	PdrVertexBuffer* const pVBPdr = mBatchedVertexBuffer;
// 
// 	const VertexAttributes& rAttributes = StaticCast<Geometry>(pVisible[min].
// 		Object)->GetMesh()->GetVertexBuffer()->GetAttributes();
// 	pVBPdr->CreateDeclaration(this, rAttributes);
// 
// 	void* pIBRaw = pIBPdr->Lock(Buffer::LM_WRITE_ONLY);
// 	void* pVBRaw = pVBPdr->Lock(Buffer::LM_WRITE_ONLY);
// 
// 	UInt offset = 0;
// 	UInt triangleCount = 0;
// 
// 	for (UInt i = min; i < max; i++)
// 	{
// 		Geometry* pGeometry = StaticCast<Geometry>(pVisible[i].Object);
// 
// 		VertexBuffer* pVertexBuffer = pGeometry->GetMesh()->GetVertexBuffer();
// 		pVBPdr->Copy(pVertexBuffer, pVBRaw, pGeometry->World);
// 		UInt size = pVertexBuffer->GetQuantity() * pVBPdr->GetVertexSize() +
// 			reinterpret_cast<UInt>(pVBRaw);
// 		pVBRaw = reinterpret_cast<void*>(size);
// 
// 		// TODO: support start and active index
// 		IndexBuffer* pIndexBuffer = pGeometry->GetMesh()->GetIndexBuffer();
// 		pIBPdr->Copy(pIndexBuffer, pIBRaw, offset);
// 		offset += pVertexBuffer->GetQuantity();
// 		size = pIndexBuffer->GetQuantity() * pIBPdr->GetIndexSize() +
// 			reinterpret_cast<UInt>(pIBRaw);
// 		pIBRaw = reinterpret_cast<void*>(size);
// 
// 		triangleCount += pIndexBuffer->GetQuantity();
// 	}
// 
// 	triangleCount = triangleCount / 3;
// 
// 	pVBPdr->Unlock();
// 	pIBPdr->Unlock();
// 
// 	Geometry* pGeometry = StaticCast<Geometry>(pVisible[min].Object);
// 	Set(pGeometry->States);
// 	Set(pGeometry->Lights);
// 	if (mspVertexBuffer)
// 	{
// 		Disable(mspVertexBuffer);
// 		mspVertexBuffer = NULL;
// 	}
// 
// 	if (mspIndexBuffer)
// 	{
// 		Disable(mspIndexBuffer);
// 		mspIndexBuffer = NULL;
// 	}
// 
// 	pIBPdr->Enable(this);
// 	pVBPdr->Enable(this);
// 
// 	Set(pGeometry->GetMaterial());
// 	
// 	Transformation identity;
// 	SetWorldTransformation(identity, pGeometry->GetMesh()->
// 		GetVertexBuffer()->GetAttributes().HasNormal());
// 
// //	DrawElements(pGeometry->ActiveIndexCount, pGeometry->StartIndex);
// 	const UInt vertexCount = offset;
// 	mStatistics.DrawCalls++;
// 	mStatistics.Triangles += triangleCount;
// 
// 	IDirect3DDevice9*& rDevice = mpData->D3DDevice;
// 	HRESULT hr;
// 	hr = rDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount,
// 		0, triangleCount);
// 	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void Renderer::Enable(StatePtr spStates[])
{
	State* pState = spStates[State::ALPHA];
	if (pState)
	{
		SetState(StaticCast<StateAlpha>(pState));
	}

	pState = spStates[State::CULL];
	if (pState)
	{
		SetState(StaticCast<StateCull>(pState));
	}

	pState = spStates[State::FOG];
	if (pState)
	{
		SetState(StaticCast<StateFog>(pState));
	}

	pState = spStates[State::MATERIAL];
	if (pState)
	{
		SetState(StaticCast<StateMaterial>(pState));
	}

	pState = spStates[State::WIREFRAME];
	if (pState)
	{
		SetState(StaticCast<StateWireframe>(pState));
	}

	pState = spStates[State::ZBUFFER];
	if (pState)
	{
		SetState(StaticCast<StateZBuffer>(pState));
	}
}

//----------------------------------------------------------------------------
void Renderer::Disable(StatePtr spStates[])
{
	State* pState;

	if (spStates[State::ALPHA])
	{
		pState = State::Default[State::ALPHA];
		SetState(StaticCast<StateAlpha>(pState));
	}

	if (spStates[State::CULL])
	{
		pState = State::Default[State::CULL];
		SetState(StaticCast<StateCull>(pState));
	}

	if (spStates[State::FOG])
	{
		pState = State::Default[State::FOG];
		SetState(StaticCast<StateFog>(pState));
	}

	if (spStates[State::MATERIAL])
	{
		pState = State::Default[State::MATERIAL];
		SetState(StaticCast<StateMaterial>(pState));
	}

	if (spStates[State::WIREFRAME])
	{
		pState = State::Default[State::WIREFRAME];
		SetState(StaticCast<StateWireframe>(pState));
	}

	if (spStates[State::ZBUFFER])
	{
		pState = State::Default[State::ZBUFFER];
		SetState(StaticCast<StateZBuffer>(pState));
	}
}

//----------------------------------------------------------------------------
void Renderer::Set(StatePtr spStates[])
{
	State* pState = spStates[State::ALPHA];
	if (pState && pState != mspStates[State::ALPHA])
	{
		SetState(StaticCast<StateAlpha>(pState));
	}

	pState = spStates[State::CULL];
	if (pState && pState != mspStates[State::CULL])
	{
		SetState(StaticCast<StateCull>(pState));
	}

	pState = spStates[State::FOG];
	if (pState && pState != mspStates[State::FOG])
	{
		SetState(StaticCast<StateFog>(pState));
	}

	pState = spStates[State::MATERIAL];
	if (pState && pState != mspStates[State::MATERIAL])
	{
		SetState(StaticCast<StateMaterial>(pState));
	}

	pState = spStates[State::WIREFRAME];
	if (pState && pState != mspStates[State::WIREFRAME])
	{
		SetState(StaticCast<StateWireframe>(pState));
	}

	pState = spStates[State::ZBUFFER];
	if (pState && pState != mspStates[State::ZBUFFER])
	{
		SetState(StaticCast<StateZBuffer>(pState));
	}
}

//----------------------------------------------------------------------------
void Renderer::Enable(const TArray<Pointer<Light> >& rLights)
{
	UInt lightCount = rLights.GetQuantity();
	if (lightCount == 0)
	{
		return;
	}

 	if (lightCount > mLights.GetQuantity())
 	{
 		lightCount = mLights.GetQuantity();
 	}
	
	ColorRGB ambient = ColorRGB::BLACK;
	for (UInt i = 0; i < lightCount; i++)
	{
		ambient += rLights[i]->Ambient;
		SetLight(rLights[i], i);
	}

	ambient.Saturate();
	EnableLighting(ambient);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const TArray<Pointer<Light> >& rLights)
{
	 UInt lightCount = rLights.GetQuantity();
	if (lightCount == 0)
	{
		return;
	}

	DisableLighting();

	if (lightCount > mLights.GetQuantity())
	{
		lightCount = mLights.GetQuantity();
	}

	for (UInt i = 0; i < lightCount; i++)
	{
		SetLight(NULL, i);
	}
}

//----------------------------------------------------------------------------
void Renderer::Set(const TArray<Pointer<Light> >& rLights)
{
	UInt lightCount = rLights.GetQuantity();
	if (lightCount > mLights.GetQuantity())
	{
		lightCount = mLights.GetQuantity();
	}

	ColorRGB ambient = ColorRGB::BLACK;
	UInt unit = 0;
	for (unit = 0; unit < lightCount; unit++)
	{
		if (mLights[unit] != rLights[unit])
		{
			SetLight(rLights[unit], unit);
		}

		ambient += rLights[unit]->Ambient;
	}

	for (; unit < mLights.GetQuantity(); unit++)
	{
		if (mLights[unit])
		{
			SetLight(NULL, unit);
		}
	}

	if (lightCount > 0)
	{
		ambient.Saturate();
		EnableLighting(ambient);
	}
	else
	{
		DisableLighting();
	}
}

//----------------------------------------------------------------------------
void Renderer::ResetStatistics()
{
	mStatistics.DrawCalls = 0;
	mStatistics.Triangles = 0;
}

//----------------------------------------------------------------------------
void Renderer::CreateBatchingBuffers(UInt size)
{
	DestroyBatchingBuffers();

	if (size == 0)
	{
		return;
	}

	mBatchedIndexBuffer = WIRE_NEW PdrIndexBuffer(this, size,
		Buffer::UT_DYNAMIC_WRITE_ONLY);
	mStatistics.IBOCount++;
	mStatistics.IBOTotalSize += mBatchedIndexBuffer->GetBufferSize();

	mBatchedVertexBuffer = WIRE_NEW PdrVertexBuffer(this, size,
		Buffer::UT_DYNAMIC_WRITE_ONLY);
	mStatistics.VBOCount++;
	mStatistics.VBOTotalSize += mBatchedVertexBuffer->GetBufferSize();
}

//----------------------------------------------------------------------------
void Renderer::DestroyBatchingBuffers()
{
	if (mBatchedIndexBuffer)
	{
		mStatistics.IBOCount--;
		mStatistics.IBOTotalSize -= mBatchedIndexBuffer->GetBufferSize();
		WIRE_DELETE mBatchedIndexBuffer;
		mBatchedIndexBuffer = NULL;
	}

	if (mBatchedVertexBuffer)
	{
		mStatistics.VBOCount--;
		mStatistics.VBOTotalSize += mBatchedVertexBuffer->GetBufferSize();
		WIRE_DELETE mBatchedVertexBuffer;
		mBatchedVertexBuffer = NULL;
	}
}