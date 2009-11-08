#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireIndexBuffer.h"
#include "WireVertexBuffer.h"
#include "WireVisibleSet.h"

using namespace Wire;

//----------------------------------------------------------------------------
Renderer::Renderer(Int width, Int height)
	:
	mWidth(width),
	mHeight(height)
{
}

//----------------------------------------------------------------------------
Renderer::~Renderer()
{
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
void Renderer::DisableIBuffer()
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
//  	pVBuffer->OnLoad(this, &Renderer::ReleaseVBuffer,pkID);
}

//----------------------------------------------------------------------------
void Renderer::EnableVBuffer()
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
// 
// 	OnEnableVBuffer(pkID);
// 	return pkID;
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pGeometry)
{
	mpGeometry = pGeometry;

	// Enable the index buffer. The connectivity information is the same
	// across all effects and all passes per effect.
	EnableIBuffer();

	EnableVBuffer();

	DrawElements();

	// Disable the index buffer.
	DisableIBuffer();
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
					Draw(static_cast<Geometry*>(pVisible[i].Object));
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
void Renderer::SetClearColor(const ColorRGBA& rClearColor)
{
	mClearColor = rClearColor;
}

