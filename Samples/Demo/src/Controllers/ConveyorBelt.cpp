#include "ConveyorBelt.h"

#include "WireMesh.h"

using namespace Wire;

//----------------------------------------------------------------------------
ConveyorBelt::ConveyorBelt(Geometry* pGeometry, Renderer* pRenderer)
	:
	mpRenderer(pRenderer),
	mOffset(0)
{
	if (!pGeometry || !pGeometry->GetMesh())
	{
		return;
	}

	Mesh* pMesh = pGeometry->GetMesh();
	for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
	{
		VertexBuffer* pVertexBuffer = pMesh->GetVertexBuffer(i);
		if (pVertexBuffer->GetAttributes().HasTCoord())
		{
			mspVertexBufferCopy = WIRE_NEW VertexBuffer(pVertexBuffer);
			break;
		}
	}

	WIRE_ASSERT(mspVertexBufferCopy);
}

//----------------------------------------------------------------------------
Bool ConveyorBelt::Update(Double appTime)
{
	Double elapsedTime = appTime - mLastApplicationTime;
	if (mLastApplicationTime == -MathD::MAX_REAL)
	{
		elapsedTime = 0;
	}

	mLastApplicationTime = appTime;

	mOffset -= static_cast<Float>(elapsedTime);
	if (mOffset < -1.0F)
	{
		mOffset += 1.0F;
	}

	return true;
}

//----------------------------------------------------------------------------
Bool ConveyorBelt::OnGetVisibleUpdate(const Camera*)
{
	Geometry* pGeo = DynamicCast<Geometry>(mpSceneObject);
	if (!mspVertexBufferCopy || !pGeo || !pGeo->GetMesh())
	{
		return false;
	}

	VertexBuffer* pVertexBuffer = NULL;
	Mesh* pMesh = pGeo->GetMesh();
	for (UInt i = 0; i < pMesh->GetVertexBuffers().GetQuantity(); i++)
	{
		if (pMesh->GetVertexBuffer(i)->GetAttributes().HasTCoord())
		{
			pVertexBuffer = pMesh->GetVertexBuffer(i);
			break;
		}
	}

	WIRE_ASSERT(pVertexBuffer);
	if (!pVertexBuffer)
	{
		return false;
	}

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
