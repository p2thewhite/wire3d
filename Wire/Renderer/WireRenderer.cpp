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

	mVertexFormatKey = 0;

	mBatchedIndexBuffer = NULL;
	mStaticBatchingThreshold = 0;
	mDynamicBatchingThreshold = 0;

	mStatistics.mpRenderer = this;
	mStatistics.Reset();

	s_pRenderer = this;
}

//----------------------------------------------------------------------------
void Renderer::Terminate()
{
	ReleaseResources();
	mspCamera = NULL;

	DestroyAll(mIndexBufferMap);
	DestroyAll(mVertexBufferMap);
	DestroyAll(mImage2DMap);
	DestroyAll(mVertexFormatMap);
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

	for (UInt i = 0; i < mVertexBuffers.GetQuantity(); i++)
	{
		if (mVertexBuffers[i])
		{
			Disable(mVertexBuffers[i], i);
		}
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

	mspMesh = NULL;
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

			for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
			{
				s_pRenderer->Bind(pMesh->GetVertexBuffer(i));
			}

			s_pRenderer->Bind(pMesh->GetVertexBuffers());

			s_pRenderer->Bind(pMesh); // Binds display lists on Wii
		}

		const Material* pMaterial = pGeometry->GetMaterial();
		if (pMaterial)
		{
			for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
			{
				s_pRenderer->Bind(pMaterial->GetTexture(i)->GetImage());
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

			for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
			{
				s_pRenderer->Unbind(pMesh->GetVertexBuffer(i));
			}
		}

		const Material* pMaterial = pGeometry->GetMaterial();
		if (pMaterial)
		{
			for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
			{
				s_pRenderer->Unbind(pMaterial->GetTexture(i)->GetImage());
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
		mStatistics.mIBOCount++;
		mStatistics.mIBOsSize += pPdrIndexBuffer->GetBufferSize();
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
		mStatistics.mIBOCount--;
		mStatistics.mIBOsSize -= (*pValue)->GetBufferSize();
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
void Renderer::Update(const IndexBuffer* pIndexBuffer, UInt count,
	UInt offset)
{
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);

	if (pValue)
	{
		(*pValue)->Update(pIndexBuffer, count, offset);
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
		mStatistics.mVBOCount++;
		mStatistics.mVBOsSize += pPdrVertexBuffer->GetBufferSize();
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
		mStatistics.mVBOCount--;
		mStatistics.mVBOsSize -= (*pValue)->GetBufferSize();
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
void Renderer::Enable(const VertexBuffer* pVertexBuffer, UInt streamIndex)
{
	/* Disable the previous VB first. */
	WIRE_ASSERT(mVertexBuffers[streamIndex] == NULL);

	WIRE_ASSERT(pVertexBuffer);
	const UInt vertexSize = pVertexBuffer->GetAttributes().GetVertexSize();
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);

	if (pValue)
	{
		(*pValue)->Enable(this, vertexSize, streamIndex);
	}
	else
	{
		PdrVertexBuffer* pPdrVertexBuffer = Bind(pVertexBuffer);
		pPdrVertexBuffer->Enable(this, vertexSize, streamIndex);
	}

	mVertexBuffers[streamIndex] = const_cast<VertexBuffer*>(pVertexBuffer);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const VertexBuffer* pVertexBuffer, UInt streamIndex)
{
	/* This VB is not enabled */
	WIRE_ASSERT(mVertexBuffers[streamIndex] == pVertexBuffer);

	WIRE_ASSERT(pVertexBuffer);
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);

	if (pValue)
	{
		(*pValue)->Disable(this, streamIndex);
	}
	else
	{
		WIRE_ASSERT(false); // Vertex buffer is not bound
	}

	mVertexBuffers[streamIndex] = NULL;
}

//----------------------------------------------------------------------------
void Renderer::Set(const VertexBuffer* pVertexBuffer, UInt streamIndex)
{
	if (mVertexBuffers[streamIndex] != pVertexBuffer)
	{
		if (mVertexBuffers[streamIndex])
		{
			Disable(mVertexBuffers[streamIndex], streamIndex);
		}

		Enable(pVertexBuffer, streamIndex);
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
void Renderer::Update(const VertexBuffer* pVertexBuffer, UInt count,
	UInt offset)
{
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);

	if (pValue)
	{
		(*pValue)->Update(pVertexBuffer, count, offset);
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
PdrTexture2D* Renderer::Bind(const Image2D* pImage)
{
	WIRE_ASSERT(pImage);
	PdrTexture2D** pValue = mImage2DMap.Find(pImage);

	if (!pValue)
	{
		PdrTexture2D* pPdrTexture = WIRE_NEW PdrTexture2D(this, pImage);
		mImage2DMap.Insert(pImage, pPdrTexture);
		mStatistics.mTextureCount++;
		mStatistics.mTexturesSize += pPdrTexture->GetBufferSize();

		if (pImage->GetUsage() == Buffer::UT_STATIC_DISCARD_ON_BIND)
		{
			const_cast<Image2D*>(pImage)->Discard();
		}

		return pPdrTexture;
	}

	return *pValue;
}

//----------------------------------------------------------------------------
void Renderer::Unbind(const Image2D* pImage)
{
	PdrTexture2D** pValue = mImage2DMap.Find(pImage);

	if (pValue)
	{
		mStatistics.mTextureCount--;
		mStatistics.mTexturesSize -= (*pValue)->GetBufferSize();
		WIRE_DELETE *pValue;
		mImage2DMap.Remove(pImage);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const Image2D* pImage)
{
	if (s_pRenderer)
	{
		s_pRenderer->Unbind(pImage);
	}
}

//----------------------------------------------------------------------------
void Renderer::Enable(const Texture2D* pTexture, UInt unit)
{
	WIRE_ASSERT(mTexture2Ds.GetQuantity() > unit);
	WIRE_ASSERT(mTexture2Ds[unit] == NULL /* Disable previous first */);
	WIRE_ASSERT(pTexture);
	const Image2D* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	PdrTexture2D** pValue = mImage2DMap.Find(pImage);

	if (pValue)
	{
		(*pValue)->Enable(this, pTexture, unit);
	}
	else
	{
		PdrTexture2D* pPdrTexture =	Bind(pImage);
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
	const Image2D* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	PdrTexture2D** pValue = mImage2DMap.Find(pImage);

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
PdrTexture2D* Renderer::GetResource(const Image2D* pImage)
{
	PdrTexture2D** pValue = mImage2DMap.Find(pImage);

	if (pValue)
	{
		return *pValue;
	}

	return NULL;
}

//----------------------------------------------------------------------------
PdrVertexFormat* Renderer::Bind(const TArray<VertexBufferPtr>& rVertexBuffers)
{
	const UInt key = GetVertexFormatKey(rVertexBuffers);
	PdrVertexFormat** pValue = mVertexFormatMap.Find(key);

	if (!pValue)
	{
		PdrVertexFormat* pPdrVertexFormat = WIRE_NEW PdrVertexFormat(this,
			rVertexBuffers);
		mVertexFormatMap.Insert(key, pPdrVertexFormat);
		mStatistics.mVertexFormatCount++;
		return pPdrVertexFormat;
	}

	return *pValue;
}

//----------------------------------------------------------------------------
UInt Renderer::GetVertexFormatKey(const TArray<VertexBufferPtr>&
	rVertexBuffers)
{
	WIRE_ASSERT(rVertexBuffers.GetQuantity() < 256);

	const UInt streamCount = rVertexBuffers.GetQuantity();
	TArray<UInt>& rBoundKeys = mVertexFormatKeys[streamCount];

	for (UInt i = 0; i < rBoundKeys.GetQuantity(); i +=streamCount)
	{
		Bool isIdentical = true;
		for (UInt j = 0; j < streamCount; j++)
		{
			if (rBoundKeys[i+j] != rVertexBuffers[j]->GetAttributes().
				GetKey())
			{
				isIdentical = false;
				break;
			}
		}

		if (isIdentical)
		{
			return (streamCount << 24) + i;
		}
	}

	rBoundKeys.SetGrowBy(streamCount);
	for (UInt i = 0; i < streamCount; i++)
	{
		rBoundKeys.Append(rVertexBuffers[i]->GetAttributes().GetKey());
	}

	return (streamCount << 24) + (rBoundKeys.GetQuantity()-streamCount);
}

//----------------------------------------------------------------------------
void Renderer::Enable(const TArray<VertexBufferPtr>& rVertexBuffers)
{
	const UInt key = GetVertexFormatKey(rVertexBuffers);
	PdrVertexFormat** pValue = mVertexFormatMap.Find(key);

	if (pValue)
	{
		(*pValue)->Enable(this);
	}
	else
	{
		PdrVertexFormat* pPdrVertexFormat = Bind(rVertexBuffers);
		pPdrVertexFormat->Enable(this);
	}

	mVertexFormatKey = key;
}

//----------------------------------------------------------------------------
void Renderer::Disable(const TArray<VertexBufferPtr>& rVertexBuffers)
{
	const UInt key = GetVertexFormatKey(rVertexBuffers);
	PdrVertexFormat** pValue = mVertexFormatMap.Find(key);

	if (pValue)
	{
		(*pValue)->Disable(this);
	}
	else
	{
		WIRE_ASSERT(false); // Vertex Attribute is not bound
	}

	mVertexFormatKey = 0;
}

//----------------------------------------------------------------------------
void Renderer::Set(const TArray<VertexBufferPtr>& rVertexBuffers)
{
	const UInt key = GetVertexFormatKey(rVertexBuffers);
	if (mVertexFormatKey != key)
	{
		if (mVertexFormatKey != 0)
		{
			PdrVertexFormat** pValue = mVertexFormatMap.Find(
				mVertexFormatKey);

			if (pValue)
			{
				(*pValue)->Disable(this);
			}
			else
			{
				WIRE_ASSERT(false); // Vertex Attribute is not bound
			}
		}

		Enable(rVertexBuffers);
	}
}

//----------------------------------------------------------------------------
PdrVertexFormat* Renderer::GetResource(const TArray<VertexBufferPtr>&
	rVertexBuffers)
{
	const UInt key = GetVertexFormatKey(rVertexBuffers);
	PdrVertexFormat** pValue = mVertexFormatMap.Find(key);

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
		const IndexBuffer* pKey = NULL;
		it.GetFirst(&pKey);
		WIRE_ASSERT(pKey);
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
		const VertexBuffer* pKey = NULL;
		it.GetFirst(&pKey);
		WIRE_ASSERT(pKey);
		Unbind(pKey);
	}

	WIRE_ASSERT(rVertexBufferMap.GetQuantity() == 0);
}

//----------------------------------------------------------------------------
void Renderer::DestroyAll(Image2DMap& rTexture2DMap)
{
	while (rTexture2DMap.GetQuantity() > 0)
	{
		Image2DMap::Iterator it(&rTexture2DMap);
		const Image2D* pKey = NULL;
		it.GetFirst(&pKey);
		WIRE_ASSERT(pKey);
		Unbind(pKey);
	}

	WIRE_ASSERT(rTexture2DMap.GetQuantity() == 0);
}

//----------------------------------------------------------------------------
void Renderer::DestroyAll(VertexFormatMap& rVertexFormatMap)
{
	while (rVertexFormatMap.GetQuantity() > 0)
	{
		VertexFormatMap::Iterator it(&rVertexFormatMap);
		UInt key;
		PdrVertexFormat** pValue = it.GetFirst(&key);

		if (pValue)
		{
			WIRE_DELETE *pValue;
			rVertexFormatMap.Remove(key);
		}
	}

	mStatistics.mVertexFormatCount = 0;
	WIRE_ASSERT(rVertexFormatMap.GetQuantity() == 0);
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
		EnableTextureStage(pMaterial->GetBlendMode(i), i, pMaterial->
			GetTexture(i)->GetImage()->HasAlpha());
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
		DisableTextureStage(i);
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
			for (UInt i = 0; i < mspMaterial->GetTextureQuantity(); i++)
			{
				DisableTextureStage(i);
			}
		}

		if (pMaterial)
		{
			for (UInt i = 0; i < pMaterial->GetTextureQuantity(); i++)
			{
				Set(pMaterial->GetTexture(i), i);
				EnableTextureStage(pMaterial->GetBlendMode(i), i, pMaterial->
					GetTexture(i)->GetImage()->HasAlpha());
			}
		}

		mspMaterial = const_cast<Material*>(pMaterial);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const Mesh* pMesh)
{
	if (s_pRenderer)
	{
		s_pRenderer->Unbind(pMesh);
	}
}

//----------------------------------------------------------------------------
void Renderer::Enable(const Mesh* pMesh)
{
	WIRE_ASSERT(pMesh);

	Enable(pMesh->GetIndexBuffer());
	Enable(pMesh->GetVertexBuffers());

	for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
	{
		Enable(pMesh->GetVertexBuffer(i), i);
	}

	mspMesh = const_cast<Mesh*>(pMesh);
}

//----------------------------------------------------------------------------
void Renderer::Disable(const Mesh* pMesh)
{
	WIRE_ASSERT(pMesh);

	for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
	{
		Disable(pMesh->GetVertexBuffer(i), i);
	}

	Disable(pMesh->GetVertexBuffers());
	Disable(pMesh->GetIndexBuffer());

	mspMesh = NULL;
}

//----------------------------------------------------------------------------
void Renderer::Set(const Mesh* pMesh)
{
	WIRE_ASSERT(pMesh);

	if (mspMesh != pMesh)
	{
		Set(pMesh->GetIndexBuffer());
		Set(pMesh->GetVertexBuffers());

		for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
		{
			Set(pMesh->GetVertexBuffer(i), i);
		}

		mspMesh = const_cast<Mesh*>(pMesh);
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
	Bool usesNormals = pMesh->HasNormal();
	if (restoreState)
	{
		Enable(pGeometry->States);
		Enable(pGeometry->Lights);
		Enable(pMesh);
		Enable(pGeometry->GetMaterial());

		SetWorldTransformation(pGeometry->World, usesNormals);
		DrawElements(pMesh->GetVertexQuantity(), pMesh->GetIndexCount(),
			pMesh->GetStartIndex());

		Disable(pGeometry->GetMaterial());
		Disable(pMesh);
		Disable(pGeometry->Lights);
		Disable(pGeometry->States);
	}
	else
	{
		Set(pGeometry->States);
		Set(pGeometry->Lights);
		Set(pMesh);
		Set(pGeometry->GetMaterial());

		SetWorldTransformation(pGeometry->World, usesNormals);
		DrawElements(pMesh->GetVertexQuantity(), pMesh->GetIndexCount(),
			pMesh->GetStartIndex());
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

	if (mStaticBatchingThreshold == 0 && mDynamicBatchingThreshold == 0)
	{
		for (UInt i = min; i < max; i++)
		{
			WIRE_ASSERT(DynamicCast<Geometry>(pVisible[i].Object));
			Draw(StaticCast<Geometry>(pVisible[i].Object), false, true);
		}

		return;
	}

	while (min < max)
	{
		UInt idx = min;
		const UInt maxStreams = mBatchedVertexBuffers.GetQuantity();
		Bool hasIdenticalVBs = true;

		WIRE_ASSERT(DynamicCast<Geometry>(pVisible[idx].Object));
		Geometry* pA = StaticCast<Geometry>(pVisible[idx].Object);
		Mesh* pMeshA = pA->GetMesh();
		WIRE_ASSERT(pMeshA);
		UInt vA = GetVertexFormatKey(pMeshA->GetVertexBuffers());

		for (; idx < max-1; idx++)
		{
			WIRE_ASSERT(DynamicCast<Geometry>(pVisible[idx+1].Object));
			Geometry* pB = StaticCast<Geometry>(pVisible[idx+1].Object);
			Mesh* pMeshB = pB->GetMesh();
			WIRE_ASSERT(pMeshB);

			if (pA->GetMaterial() != pB->GetMaterial() ||
				pA->StateSetID != pB->StateSetID)
			{
				break;
			}

			const TArray<VertexBufferPtr>& rVBOsB = pMeshB->GetVertexBuffers();
			UInt vB = GetVertexFormatKey(rVBOsB);
			if ((vA != vB))
			{
				break;
			}

			Bool hadIdenticalVBs = hasIdenticalVBs;
			for (UInt i = 0; i < rVBOsB.GetQuantity(); i++)
			{
				const VertexBufferPtr* pVBOsA = pMeshA->GetVertexBuffers().
					GetArray();
				if (rVBOsB[i] != pVBOsA[i])
				{
					hasIdenticalVBs = false;
					break;
				}
			}

			if (hadIdenticalVBs && !hasIdenticalVBs && (idx > min))
			{
				hasIdenticalVBs = true;
				break;
			}

			if (!hasIdenticalVBs && rVBOsB.GetQuantity() > maxStreams)
			{
				break;
			}

  			pA = pB;
  			vA = vB;
		}

		if (idx > min)
		{
			BatchAndDraw(pVisible, min, idx+1);
		}
		else
		{
			WIRE_ASSERT(DynamicCast<Geometry>(pVisible[min].Object));
			Draw(StaticCast<Geometry>(pVisible[min].Object), false, true);
		}

		min = idx+1;
	}
}

//----------------------------------------------------------------------------
void Renderer::BatchAndDraw(VisibleObject* const pVisible, UInt min, UInt max)
{
	Geometry* pGeometry = StaticCast<Geometry>(pVisible[min].Object);
	const TArray<VertexBufferPtr>& rVertexBuffers = pGeometry->GetMesh()->
		GetVertexBuffers();
	Bind(rVertexBuffers);
 	
	PdrIndexBuffer* const pIBPdr = mBatchedIndexBuffer;
	void* pIBRaw = pIBPdr->Lock(Buffer::LM_WRITE_ONLY);

	const UInt vbCount = rVertexBuffers.GetQuantity();
	for (UInt i = 0; i < vbCount; i++)
	{
		mRawBatchedVertexBuffers[i] = mBatchedVertexBuffers[i]->Lock(
			Buffer::LM_WRITE_ONLY);
	}

	UShort batchedVertexCount = 0;
	UInt batchedIndexCount = 0;

	for (UInt i = min; i < max; i++)
	{
		Geometry* pGeometry = StaticCast<Geometry>(pVisible[i].Object);
		Mesh* const pMesh = pGeometry->GetMesh();

		IndexBuffer* const pIndexBuffer = pMesh->GetIndexBuffer();
		const UInt ibSize = pMesh->GetIndexCount() * sizeof(UShort);

		WIRE_ASSERT(vbCount <= mBatchedVertexBuffers.GetQuantity());
		const UInt vertexCount = pMesh->GetVertexBuffer()->GetQuantity();

		Bool exceedsBuffer = ibSize > mBatchedIndexBuffer->GetBufferSize()
			|| (batchedVertexCount + vertexCount > 0xFFFF);
		for (UInt j = 0; (j < vbCount) && !exceedsBuffer; j++)
		{
			VertexBuffer* const pVertexBuffer = pMesh->GetVertexBuffer(j);
			UInt vertexSize = pVertexBuffer->GetAttributes().GetVertexSize();
			UInt vbSize = vertexCount * vertexSize;
			exceedsBuffer = exceedsBuffer
				|| (vbSize > mBatchedVertexBuffers[j]->GetBufferSize());
		}

		if (exceedsBuffer)
		{
			Draw(pGeometry, false, true);
			continue;
		}

		if (pGeometry->World.IsIdentity())
		{
			if (vertexCount > mStaticBatchingThreshold)
			{
				Draw(pGeometry, false, true);
				continue;
			}

			mStatistics.mBatchedStatic++;
		}
		else
		{
			if (vertexCount > mDynamicBatchingThreshold)
			{
				Draw(pGeometry, false, true);
				continue;
			}

			mStatistics.mBatchedDynamic++;
		}

		exceedsBuffer = (ibSize+batchedIndexCount*sizeof(UShort)) >
			mBatchedIndexBuffer->GetBufferSize();
		for (UInt j = 0; (j < vbCount) && !exceedsBuffer; j++)
		{
			VertexBuffer* const pVertexBuffer = pMesh->GetVertexBuffer(j);
			UInt vertexSize = pVertexBuffer->GetAttributes().GetVertexSize();
			UInt vbSize = (vertexCount+batchedVertexCount) * vertexSize;
			exceedsBuffer = exceedsBuffer
				|| vbSize > mBatchedVertexBuffers[j]->GetBufferSize();
		}

		if (exceedsBuffer)
		{
			for (UInt i = 0; i < vbCount; i++)
			{
				mBatchedVertexBuffers[i]->Unlock();
			}

			pIBPdr->Unlock();

			Geometry* pGeometry = StaticCast<Geometry>(pVisible[min].Object);
			Draw(pGeometry, pIBPdr, mBatchedVertexBuffers, batchedVertexCount,
				batchedIndexCount);
			mStatistics.mBatchCount++;

			pIBRaw = pIBPdr->Lock(Buffer::LM_WRITE_ONLY);
			for (UInt i = 0; i < vbCount; i++)
			{
				mRawBatchedVertexBuffers[i] = mBatchedVertexBuffers[i]->Lock(
					Buffer::LM_WRITE_ONLY);
			}

			batchedVertexCount = 0;
			batchedIndexCount = 0;
		}

		pIndexBuffer->Copy(static_cast<UShort*>(pIBRaw), batchedVertexCount,
			pMesh->GetIndexCount(), pMesh->GetStartIndex());
		pIBRaw = reinterpret_cast<void*>(ibSize + reinterpret_cast<UInt>(
			pIBRaw));
		mStatistics.mBatchedIBOData += ibSize;

		for (UInt j = 0; j < vbCount; j++)
		{
			VertexBuffer* const pVertexBuffer = pMesh->GetVertexBuffer(j);
			pVertexBuffer->ApplyForward(pGeometry->World, static_cast<Float*>(
				mRawBatchedVertexBuffers[j]));

			UInt vertexSize = pVertexBuffer->GetAttributes().GetVertexSize();
			UInt vbSize = vertexCount * vertexSize;
			mRawBatchedVertexBuffers[j] = reinterpret_cast<void*>(vbSize +
				reinterpret_cast<UInt>(mRawBatchedVertexBuffers[j]));
			mStatistics.mBatchedVBOData += vbSize;
		}

		batchedVertexCount += static_cast<UShort>(vertexCount);
		batchedIndexCount += pMesh->GetIndexCount();
	}

	pIBPdr->Unlock();
	for (UInt i = 0; i < vbCount; i++)
	{
		mBatchedVertexBuffers[i]->Unlock();
	}

	if (batchedIndexCount > 0)
	{
		Geometry* pGeometry = StaticCast<Geometry>(pVisible[min].Object);
		Draw(pGeometry, pIBPdr, mBatchedVertexBuffers, batchedVertexCount,
			batchedIndexCount);
		mStatistics.mBatchCount++;
	}
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pUseStateFrom, PdrIndexBuffer* const pIBPdr,
	TArray<PdrVertexBuffer*>& rVBsPdr, UShort vertexCount, UInt indexCount)
{
	Set(pUseStateFrom->States);
	Set(pUseStateFrom->Lights);

	for (UInt i = 0; i < mVertexBuffers.GetQuantity(); i++)
	{
		if (mVertexBuffers[i])
		{
			Disable(mVertexBuffers[i], i);
		}
	}

	if (mspIndexBuffer)
	{
		Disable(mspIndexBuffer);
		mspIndexBuffer = NULL;
	}

	pIBPdr->Enable(this);

	Mesh* const pMesh = pUseStateFrom->GetMesh();
	const TArray<VertexBufferPtr>& rVertexBuffers = pMesh->GetVertexBuffers();
	Set(pMesh->GetVertexBuffers());

	for (UInt i = 0; i < rVertexBuffers.GetQuantity(); i++)
	{
		UInt vertexSize = rVertexBuffers[i]->GetAttributes().GetVertexSize();
		rVBsPdr[i]->Enable(this, vertexSize, i);
	}

	Set(pUseStateFrom->GetMaterial());
	Transformation identity;
	SetWorldTransformation(identity, pMesh->HasNormal());

	DrawElements(vertexCount, indexCount, 0);

	for (UInt i = 0; i < rVertexBuffers.GetQuantity(); i++)
	{
		rVBsPdr[i]->Disable(this, i);
	}

	pIBPdr->Disable(this);
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
void Renderer::CreateBatchingBuffers(UInt size, UInt maxVertexStreams)
{
	DestroyBatchingBuffers();

	if (size == 0 || maxVertexStreams == 0)
	{
		return;
	}

	if (maxVertexStreams > mVertexBuffers.GetQuantity())
	{
		maxVertexStreams = mVertexBuffers.GetQuantity();
	}

	mBatchedIndexBuffer = WIRE_NEW PdrIndexBuffer(this, size,
		Buffer::UT_DYNAMIC_WRITE_ONLY);
	mStatistics.mIBOCount++;
	mStatistics.mIBOsSize += mBatchedIndexBuffer->GetBufferSize();

	mBatchedVertexBuffers.SetQuantity(maxVertexStreams);
	mRawBatchedVertexBuffers.SetQuantity(maxVertexStreams);
	for (UInt i = 0; i < maxVertexStreams; i++)
	{
		mBatchedVertexBuffers[i] =  WIRE_NEW PdrVertexBuffer(this, size,
			Buffer::UT_DYNAMIC_WRITE_ONLY);
		const UInt bufferSize = mBatchedVertexBuffers[i]->GetBufferSize();
		mStatistics.mVBOCount++;
		mStatistics.mVBOsSize += bufferSize;
		mStatistics.mBatchVBOCount++;
		mStatistics.mBatchVBOsSize += bufferSize;
	}
}

//----------------------------------------------------------------------------
void Renderer::DestroyBatchingBuffers()
{
	if (mBatchedIndexBuffer)
	{
		mStatistics.mIBOCount--;
		mStatistics.mIBOsSize -= mBatchedIndexBuffer->GetBufferSize();
		WIRE_DELETE mBatchedIndexBuffer;
		mBatchedIndexBuffer = NULL;
	}

	for (UInt i = 0; i < mBatchedVertexBuffers.GetQuantity(); i++)
	{
		const UInt bufferSize = mBatchedVertexBuffers[i]->GetBufferSize();
		mStatistics.mVBOCount--;
		mStatistics.mVBOsSize -= bufferSize;
		mStatistics.mBatchVBOCount--;
		mStatistics.mBatchVBOsSize -= bufferSize;
		WIRE_DELETE mBatchedVertexBuffers[i];
		mBatchedVertexBuffers[i] = NULL;
	}

	mBatchedVertexBuffers.SetQuantity(0);
	mRawBatchedVertexBuffers.SetQuantity(0);
}
