#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireIndexBuffer.h"
#include "WireTexture2D.h"
#include "WireTextureEffect.h"
#include "WireVertexBuffer.h"
#include "WireVisibleSet.h"

#ifdef WIRE_WII
#include "WireGXTexture2D.h"
#else
#include "WireDx9Texture2D.h"
#endif

using namespace Wire;

Renderer* Renderer::smRenderer = NULL;

//----------------------------------------------------------------------------
void Renderer::Initialize(UInt width, UInt height)
{
	mWidth = width;
	mHeight = height;
	mCurrentSampler = 0;
	mMaxAnisotropy = 1.0F;
	smRenderer = this;
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
void Renderer::LoadIBuffer(IndexBuffer* pIBuffer)
{
 	if (!pIBuffer)
	{
		return;
	}

 	ResourceIdentifier* pID = pIBuffer->GetIdentifier(this);
	if (!pID)
	{
		OnLoadIBuffer(pID, pIBuffer);
		pIBuffer->OnLoad(this, &Renderer::ReleaseIBuffer, pID);
	}
}

//----------------------------------------------------------------------------
void Renderer::ReleaseIBuffer(Bindable* pIBuffer)
{
	if (!pIBuffer)
	{
		return;
	}

	ResourceIdentifier* pID = pIBuffer->GetIdentifier(this);
	if (pID)
	{
		OnReleaseIBuffer(pID);
		pIBuffer->OnRelease(this, pID);
	}
}

//----------------------------------------------------------------------------
void Renderer::EnableIBuffer ()
{
 	IndexBuffer* pIBuffer = mpGeometry->IBuffer;
	// TODO: have LoadIBuffer return pID
 	LoadIBuffer(pIBuffer);
 	ResourceIdentifier* pID = pIBuffer->GetIdentifier(this);
 	WIRE_ASSERT(pID);
 	OnEnableIBuffer(pID);
}

//----------------------------------------------------------------------------
void Renderer::DisableIBuffer ()
{
	IndexBuffer* pIBuffer = mpGeometry->IBuffer;
	ResourceIdentifier* pID = pIBuffer->GetIdentifier(this);
	WIRE_ASSERT(pID);
	OnDisableIBuffer(pID);
}

//----------------------------------------------------------------------------
void Renderer::LoadVBuffer(VertexBuffer* pVBuffer)
{
	if (!pVBuffer)
	{
		return;
	}

 	// Search for a matching vertex buffer that was used during previous
 	// passes.
 	ResourceIdentifier* pID = 0;
 	for (UInt i = 0; i < pVBuffer->GetInfoQuantity(); i++)
 	{
 		pID = pVBuffer->GetIdentifier(i, this);
 		if (pID)
 		{
// 			if (rkIAttr == *(Attributes*)pkID)
 			{
 				// Found a matching vertex buffer in video memory.
 				return;
 			}
 		}
 	}
 
 	// The vertex buffer is encountered the first time.
 	OnLoadVBuffer(pID, pVBuffer);
  	pVBuffer->OnLoad(this, &Renderer::ReleaseVBuffer, pID);
}

//----------------------------------------------------------------------------
void Renderer::ReleaseVBuffer(Bindable* pVBuffer)
{
	if (!pVBuffer)
	{
		return;
	}

	for (UInt i = 0; i < pVBuffer->GetInfoQuantity(); i++)
	{
		ResourceIdentifier* pID = pVBuffer->GetIdentifier(i, this);
		if (pID)
		{
			OnReleaseVBuffer(pID);
			pVBuffer->OnRelease(this, pID);
			return;
		}
	}
}

//----------------------------------------------------------------------------
ResourceIdentifier* Renderer::EnableVBuffer()
{
	VertexBuffer* pVBuffer = mpGeometry->VBuffer;
	LoadVBuffer(pVBuffer);

 	ResourceIdentifier* pID = 0;
 	for (UInt i = 0; i < pVBuffer->GetInfoQuantity(); i++)
 	{
 		pID = pVBuffer->GetIdentifier(i, this);
 		if (pID)
		{
//			if (rkIAttr == *(Attributes*)pkID)
			{
				// Found a matching vertex buffer in video memory.
				break;
			}
		}
	}

 	WIRE_ASSERT(pID);
 
	OnEnableVBuffer(pID);
 	return pID;
}

//----------------------------------------------------------------------------
void Renderer::DisableVBuffer(ResourceIdentifier* pID)
{
	OnDisableVBuffer(pID);
}

//----------------------------------------------------------------------------
void Renderer::LoadTexture(Texture2D* pTexture)
{
	if (!pTexture)
	{
		return;
	}

	ResourceIdentifier* pID = pTexture->GetIdentifier(this);
	if (!pID)
	{
		OnLoadTexture(pID, pTexture);
		pTexture->OnLoad(this, &Renderer::ReleaseTexture, pID);
	}
}

//----------------------------------------------------------------------------
void Renderer::ReleaseTexture(Bindable* pTexture)
{
	if (!pTexture)
	{
		return;
	}

	ResourceIdentifier* pID = pTexture->GetIdentifier(this);
	if (pID)
	{
		OnReleaseTexture(pID);
		pTexture->OnRelease(this, pID);
	}
}

//----------------------------------------------------------------------------
void Renderer::EnableTexture(Texture2D* pTexture)
{
	WIRE_ASSERT(pTexture);
// 	LoadTexture(pTexture);
// 	ResourceIdentifier* pID = pTexture->GetIdentifier(this);
// 	WIRE_ASSERT(pID);
// 	OnEnableTexture(pID);

	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);
	if (pValue)
	{
		(*pValue)->Enable(this, pTexture, mCurrentSampler);
	}
	else
	{
		PdrTexture2D* pPdrTexture =	Bind(pTexture);
		pPdrTexture->Enable(this, pTexture, mCurrentSampler);
	}
}

//----------------------------------------------------------------------------
void Renderer::DisableTexture(Texture2D* pTexture)
{
// 	ResourceIdentifier* pID = pTexture->GetIdentifier(this);
// 	WIRE_ASSERT(pID);
// 	OnDisableTexture(pID);

	WIRE_ASSERT(pTexture);
	PdrTexture2D** pValue = mTexture2DMap.Find(pTexture);
	if (pValue)
	{
		(*pValue)->Disable(this, mCurrentSampler);
	}
	else
	{
		WIRE_ASSERT(false); // Texture is not bound
	}
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pGeometry)
{
	mpGeometry = pGeometry;

	SetGlobalState(mpGeometry->States);

	// Enable the index buffer. The connectivity information is the same
	// across all effects and all passes per effect.
	EnableIBuffer();
	ResourceIdentifier* pID = EnableVBuffer();

	// Keep track of the current sampler to be used in enabling the
	// textures.
	mCurrentSampler = 0;

	Texture2D* pTexture = NULL;
	if (mpGeometry->GetEffectQuantity() > 0)
	{
		TextureEffect* pTextureEffect = DynamicCast<TextureEffect>(
			mpGeometry->GetEffect(0));

		if (pTextureEffect)
		{
			if (pTextureEffect->Textures.GetQuantity() > 0)
			{
				pTexture = pTextureEffect->Textures[0];
				WIRE_ASSERT(pTexture);
			}
		}

		EnableTexture(pTexture);
		mCurrentSampler++;
	}

	DrawElements();

	// Keep track of the current sampler to be used in disabling the
	// textures.
	mCurrentSampler = 0;
	if (pTexture)
	{
		DisableTexture(pTexture);
	}


	DisableVBuffer(pID);
	DisableIBuffer();

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
void Renderer::SetGlobalState(GlobalStatePtr spStates[])
{
	GlobalState* pState = spStates[GlobalState::ALPHA];
	if (pState)
	{
		SetAlphaState(StaticCast<AlphaState>(pState));
	}

	pState = spStates[GlobalState::CULL];
	if (pState)
	{
		SetCullState(StaticCast<CullState>(pState));
	}

	pState = spStates[GlobalState::FOG];
	if (pState)
	{
		SetFogState(StaticCast<FogState>(pState));
	}

	pState = spStates[GlobalState::WIREFRAME];
	if (pState)
	{
		SetWireframeState(StaticCast<WireframeState>(pState));
	}

	pState = spStates[GlobalState::ZBUFFER];
	if (pState)
	{
		SetZBufferState(StaticCast<ZBufferState>(pState));
	}
}

//----------------------------------------------------------------------------
void Renderer::RestoreGlobalState(GlobalStatePtr spStates[])
{
	GlobalState* pState;

	if (spStates[GlobalState::ALPHA])
	{
		pState = GlobalState::Default[GlobalState::ALPHA];
		SetAlphaState(StaticCast<AlphaState>(pState));
	}

	if (spStates[GlobalState::CULL])
	{
		pState = GlobalState::Default[GlobalState::CULL];
		SetCullState(StaticCast<CullState>(pState));
	}

	if (spStates[GlobalState::FOG])
	{
		pState = GlobalState::Default[GlobalState::FOG];
		SetFogState(StaticCast<FogState>(pState));
	}

	if (spStates[GlobalState::WIREFRAME])
	{
		pState = GlobalState::Default[GlobalState::WIREFRAME];
		SetWireframeState(StaticCast<WireframeState>(pState));
	}

	if (spStates[GlobalState::ZBUFFER])
	{
		pState = GlobalState::Default[GlobalState::ZBUFFER];
		SetZBufferState(StaticCast<ZBufferState>(pState));
	}
}
