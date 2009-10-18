#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireIndexBuffer.h"
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
 	LoadIBuffer(pIBuffer);
 	ResourceIdentifier* pID = pIBuffer->GetIdentifier(this);
 	WIRE_ASSERT(pID);
 	OnEnableIBuffer(pID);
}

//----------------------------------------------------------------------------
void Renderer::Draw(Geometry* pGeometry)
{
	mpGeometry = pGeometry;

	// Enable the index buffer. The connectivity information is the same
	// across all effects and all passes per effect.
//	EnableIBuffer();

	DrawElements();
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

//----------------------------------------------------------------------------
void Renderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	mViewMatrix[0][0] = rVector[0];
	mViewMatrix[1][0] = uVector[0];
	mViewMatrix[2][0] = dVector[0];
	mViewMatrix[0][1] = rVector[1];
	mViewMatrix[1][1] = uVector[1];
	mViewMatrix[2][1] = dVector[1];
	mViewMatrix[0][2] = rVector[2];
	mViewMatrix[1][2] = uVector[2];
	mViewMatrix[2][2] = dVector[2];
	mViewMatrix[0][3] = -rVector.Dot(eye);
	mViewMatrix[1][3] = -uVector.Dot(eye);
	mViewMatrix[2][3] = -dVector.Dot(eye);
}
