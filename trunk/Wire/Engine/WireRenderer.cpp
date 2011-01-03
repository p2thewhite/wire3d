#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireIndexBuffer.h"
#include "WireNode.h"
#include "WireTexture2D.h"
#include "WireTextureEffect.h"
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

Renderer* Renderer::smRenderer = NULL;

//----------------------------------------------------------------------------
void Renderer::Initialize(UInt width, UInt height)
{
	mWidth = width;
	mHeight = height;
	mMaxAnisotropy = 1.0F;
	smRenderer = this;
	mpCamera = NULL;
	mpGeometry = NULL;

	mspDefaultStates[StateGlobal::ALPHA] = WIRE_NEW StateAlpha;
	mspDefaultStates[StateGlobal::CULL] = WIRE_NEW StateCull;
	mspDefaultStates[StateGlobal::FOG] = WIRE_NEW StateFog;
	mspDefaultStates[StateGlobal::MATERIAL] = WIRE_NEW StateMaterial;
	mspDefaultStates[StateGlobal::WIREFRAME] = WIRE_NEW StateWireframe;
	mspDefaultStates[StateGlobal::ZBUFFER] = WIRE_NEW StateZBuffer;
}

//----------------------------------------------------------------------------
void Renderer::Terminate()
{
	DestroyAllIndexBuffers();
	DestroyAllVertexBuffers();
	DestroyAllTexture2Ds();
}

//----------------------------------------------------------------------------
void Renderer::BindAll(const Spatial* pSpatial)
{
	if (!pSpatial)
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
		smRenderer->Bind(pGeometry->IBuffer);
		smRenderer->Bind(pGeometry->VBuffer);

		for (UInt i = 0; i < pGeometry->GetEffectQuantity(); i++)
		{
			TextureEffect* pEffect = DynamicCast<TextureEffect>(
				pGeometry->GetEffect(i));

			if (pEffect)
			{
				for (UInt j = 0; j < pEffect->Textures.GetQuantity(); j++)
				{
					smRenderer->Bind(pEffect->Textures[j]);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const Spatial* pSpatial)
{
	if (!pSpatial)
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
		smRenderer->Unbind(pGeometry->IBuffer);
		smRenderer->Unbind(pGeometry->VBuffer);

		for (UInt i = 0; i < pGeometry->GetEffectQuantity(); i++)
		{
			TextureEffect* pEffect = DynamicCast<TextureEffect>(
				pGeometry->GetEffect(i));

			if (pEffect)
			{
				for (UInt j = 0; j < pEffect->Textures.GetQuantity(); j++)
				{
					smRenderer->Unbind(pEffect->Textures[j]);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
PdrIndexBuffer* Renderer::Bind(const IndexBuffer* pIndexBuffer)
{
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);

	if (!pValue)
	{
		PdrIndexBuffer* pPdrIndexBuffer = WIRE_NEW PdrIndexBuffer(this,
			pIndexBuffer);
		mIndexBufferMap.Insert(pIndexBuffer, pPdrIndexBuffer);
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
		WIRE_DELETE *pValue;
		mIndexBufferMap.Remove(pIndexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const IndexBuffer* pIndexBuffer)
{
	smRenderer->Unbind(pIndexBuffer);
}

//----------------------------------------------------------------------------
void Renderer::Enable(const IndexBuffer* pIndexBuffer)
{
	WIRE_ASSERT(pIndexBuffer);
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);
	if (pValue)
	{
		(*pValue)->Enable(this, pIndexBuffer);
	}
	else
	{
		PdrIndexBuffer* pPdrTexture = Bind(pIndexBuffer);
		pPdrTexture->Enable(this, pIndexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::Disable(const IndexBuffer* pIndexBuffer)
{
	WIRE_ASSERT(pIndexBuffer);
	PdrIndexBuffer** pValue = mIndexBufferMap.Find(pIndexBuffer);
	if (pValue)
	{
		(*pValue)->Disable(this);
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
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);

	if (!pValue)
	{
		PdrVertexBuffer* pPdrVertexBuffer = WIRE_NEW PdrVertexBuffer(this,
			pVertexBuffer);
		mVertexBufferMap.Insert(pVertexBuffer, pPdrVertexBuffer);
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
		WIRE_DELETE *pValue;
		mVertexBufferMap.Remove(pVertexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const VertexBuffer* pVertexBuffer)
{
	smRenderer->Unbind(pVertexBuffer);
}

//----------------------------------------------------------------------------
void Renderer::Enable(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(pVertexBuffer);
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);
	if (pValue)
	{
		(*pValue)->Enable(this, pVertexBuffer);
	}
	else
	{
		PdrVertexBuffer* pPdrTexture =	Bind(pVertexBuffer);
		pPdrTexture->Enable(this, pVertexBuffer);
	}
}

//----------------------------------------------------------------------------
void Renderer::Disable(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(pVertexBuffer);
	PdrVertexBuffer** pValue = mVertexBufferMap.Find(pVertexBuffer);
	if (pValue)
	{
		(*pValue)->Disable(this);
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
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);

	if (!pValue)
	{
		PdrTexture2D* pPdrTexture = WIRE_NEW PdrTexture2D(this, pTexture);
		mTexture2DMap.Insert(pTexture, pPdrTexture);
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
		WIRE_DELETE *pValue;
		mTexture2DMap.Remove(pTexture);
	}
}

//----------------------------------------------------------------------------
void Renderer::UnbindAll(const Texture2D* pTexture)
{
	smRenderer->Unbind(pTexture);
}

//----------------------------------------------------------------------------
void Renderer::Enable(Texture2D* pTexture, UInt unit)
{
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
}

//----------------------------------------------------------------------------
void Renderer::Disable(Texture2D* pTexture, UInt unit)
{
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
void Renderer::DestroyAllIndexBuffers()
{
 	for (PdrIndexBuffer** pValue = mIndexBufferMap.GetFirst(); pValue;
		pValue = mIndexBufferMap.GetNext())
 	{
 		WIRE_DELETE *pValue;
 	}

	mIndexBufferMap.RemoveAll();
}

//----------------------------------------------------------------------------
void Renderer::DestroyAllVertexBuffers()
{
	for (PdrVertexBuffer** pValue = mVertexBufferMap.GetFirst(); pValue;
		pValue = mVertexBufferMap.GetNext())
	{
		WIRE_DELETE *pValue;
	}

	mVertexBufferMap.RemoveAll();
}

//----------------------------------------------------------------------------
void Renderer::DestroyAllTexture2Ds()
{
	for (PdrTexture2D** pValue = mTexture2DMap.GetFirst(); pValue;
		pValue = mTexture2DMap.GetNext())
	{
		WIRE_DELETE *pValue;
	}

	mTexture2DMap.RemoveAll();
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pGeometry)
{
	mpGeometry = pGeometry;

	SetGlobalState(mpGeometry->States);
	Enable(mpGeometry->Lights);

	// Enable the index buffer. The connectivity information is the same
	// across all effects and all passes per effect.
	Enable(mpGeometry->IBuffer);
	Enable(mpGeometry->VBuffer);

	UInt effectCount = mpGeometry->GetEffectQuantity();

	if (effectCount == 0)
	{
		DrawElements();
	}

	for (UInt i = 0; i < effectCount; i++)
	{
		ApplyEffect(mpGeometry->GetEffect(i));
	}

	Disable(mpGeometry->VBuffer);
	Disable(mpGeometry->IBuffer);
	Disable(mpGeometry->Lights);

	RestoreGlobalState(mpGeometry->States);
}

//----------------------------------------------------------------------------
void Renderer::DrawScene(VisibleSet& rVisibleSet)
{
	// NOTE: The stack of 2-tuples is limited to having 64 elements. This
	// should be plenty, because the chances of having 64 global effects
	// in the same path is small (that is a *lot* of effects to apply in
	// one frame). If it needs to be larger for your applications, increase
	// the maximum size.
	const Int maxTuples = 64;    // maximum number of stack elements
	Int indexStack[maxTuples][2];  // elements are (startIndex, finalIndex)
	Int top = -1;                // stack is initially empty

	const UInt visibleQuantity = rVisibleSet.GetQuantity();
	VisibleObject* pVisible = rVisibleSet.GetVisible();
	for (UInt i = 0; i < visibleQuantity; i++)
	{
		if (pVisible[i].Object)
		{
// TODO: support Effects
// 			if (pVisible[i].GlobalEffect)
// 			{
// 				// Begin the scope of a global effect.
// 				top++;
// 				assert(top < maxTuples);
// 				indexStack[top][0] = i;
// 				indexStack[top][1] = i;
// 			}
// 			else
			{
				// Found a leaf Geometry object.
				if (top == -1)
				{
					Draw(StaticCast<Geometry>(pVisible[i].Object));
				}
				else
				{
					indexStack[top][1]++;
				}
			}
		}
// 		else
// 		{
// 			// End the scope of a global effect.
//			WIRE_ASSERT(!pVisible[i].GlobalEffect);
// 			Int min = indexStack[top][0];
// 			Int max = indexStack[top][1];
// 
// 			pVisible[min].GlobalEffect->Draw(this, pVisible[min].Object,
// 				min+1,max,pVisible);
// 
// 			if (--top >= 0)
// 			{
// 				indexStack[top][1] = max + 1;
// 			}
// 		}
	}
}

//----------------------------------------------------------------------------
void Renderer::SetGlobalState(StateGlobalPtr spStates[])
{
	StateGlobal* pState = spStates[StateGlobal::ALPHA];
	if (pState)
	{
		SetState(StaticCast<StateAlpha>(pState));
	}

	pState = spStates[StateGlobal::CULL];
	if (pState)
	{
		SetState(StaticCast<StateCull>(pState));
	}

	pState = spStates[StateGlobal::FOG];
	if (pState)
	{
		SetState(StaticCast<StateFog>(pState));
	}

	pState = spStates[StateGlobal::MATERIAL];
	if (pState)
	{
		SetState(StaticCast<StateMaterial>(pState));
	}

	pState = spStates[StateGlobal::WIREFRAME];
	if (pState)
	{
		SetState(StaticCast<StateWireframe>(pState));
	}

	pState = spStates[StateGlobal::ZBUFFER];
	if (pState)
	{
		SetState(StaticCast<StateZBuffer>(pState));
	}
}

//----------------------------------------------------------------------------
void Renderer::RestoreGlobalState(StateGlobalPtr spStates[])
{
	StateGlobal* pState;

	if (spStates[StateGlobal::ALPHA])
	{
		pState = mspDefaultStates[StateGlobal::ALPHA];
		SetState(StaticCast<StateAlpha>(pState));
	}

	if (spStates[StateGlobal::CULL])
	{
		pState = mspDefaultStates[StateGlobal::CULL];
		SetState(StaticCast<StateCull>(pState));
	}

	if (spStates[StateGlobal::FOG])
	{
		pState = mspDefaultStates[StateGlobal::FOG];
		SetState(StaticCast<StateFog>(pState));
	}

	if (spStates[StateGlobal::MATERIAL])
	{
		pState = mspDefaultStates[StateGlobal::MATERIAL];
		SetState(StaticCast<StateMaterial>(pState));
	}

	if (spStates[StateGlobal::WIREFRAME])
	{
		pState = mspDefaultStates[StateGlobal::WIREFRAME];
		SetState(StaticCast<StateWireframe>(pState));
	}

	if (spStates[StateGlobal::ZBUFFER])
	{
		pState = mspDefaultStates[StateGlobal::ZBUFFER];
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

	EnableLighting();

 	if (lightCount > mMaxLights)
 	{
 		lightCount = mMaxLights;
 	}
	
	for (UInt i = 0; i < lightCount; i++)
	{
		SetLight(rLights[i], i);
	}
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

	if (lightCount > mMaxLights)
	{
		lightCount = mMaxLights;
	}

	for (UInt i = 0; i < lightCount; i++)
	{
		SetLight(NULL, i);
	}
}

//----------------------------------------------------------------------------
void Renderer::ApplyEffect(Effect* pEffect)
{
	TextureEffect* pTextureEffect = DynamicCast<TextureEffect>(pEffect);
	if (pTextureEffect)
	{
		for (UInt i = 0; i < pTextureEffect->Textures.GetQuantity(); i++)
		{
			Enable(pTextureEffect->Textures[i], i);
		}

		ApplyEffect(pTextureEffect);
		DrawElements();

		for (UInt i = 0; i < pTextureEffect->Textures.GetQuantity(); i++)
		{
			Disable(pTextureEffect->Textures[i], i);
		}
	}
}
