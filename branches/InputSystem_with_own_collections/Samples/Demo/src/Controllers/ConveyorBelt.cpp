#include "ConveyorBelt.h"

#include "WireMesh.h"

using namespace Wire;

//----------------------------------------------------------------------------
ConveyorBelt::ConveyorBelt(Geometry* pGeometry, Renderer* pRenderer)
	:
	mOffset(0)
{
	if (!pGeometry)
	{
		return;
	}

	mpRenderer = pRenderer;

	VertexBuffer* pVBuffer = pGeometry->GetMesh()->GetVertexBuffer();
	mspVertexBufferCopy = WIRE_NEW VertexBuffer(pVBuffer);
}

//----------------------------------------------------------------------------
Bool ConveyorBelt::Update(Double appTime)
{
	Double elapsedTime = appTime - mLastAppTime;
	if (mLastAppTime == -MathD::MAX_REAL)
	{
		elapsedTime = 0;
	}

	mLastAppTime = appTime;

	mOffset -= static_cast<Float>(elapsedTime);
	if (mOffset < -1.0f)
	{
		mOffset += 1.0f;
	}

	return true;
}

//----------------------------------------------------------------------------
Bool ConveyorBelt::RenderUpdate()
{
	Geometry* pGeo = DynamicCast<Geometry>(mpObject);
	if (!mspVertexBufferCopy || !pGeo)
	{
		return false;
	}

	VertexBuffer* pVertexBuffer = pGeo->GetMesh()->GetVertexBuffer();
	for (UInt i = 0; i < mspVertexBufferCopy->GetQuantity(); i++)
	{
		Vector2F vec2 = mspVertexBufferCopy->TCoord2(i);
		vec2.Y() += mOffset;
		pVertexBuffer->TCoord2(i) = vec2;
	}

	PdrVertexBuffer* pPdrVBuffer = mpRenderer->GetResource(pVertexBuffer);
	if (pPdrVBuffer)
	{
		// vertex buffer is bound, so we can update it
		mpRenderer->Update(pVertexBuffer);
	}

	return true;
}
