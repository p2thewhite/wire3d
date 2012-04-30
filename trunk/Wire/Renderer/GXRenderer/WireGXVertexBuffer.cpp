// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXVertexBuffer.h"

#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireGXRendererData.h"
#include "WireLight.h"
#include "WireMesh.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer*, const VertexBuffer* pVertexBuffer)
	:
	mVertexSize(0)
{
	CreateDeclaration(NULL, pVertexBuffer->GetAttributes());
	CreateBuffer(NULL, mVertexSize * pVertexBuffer->GetQuantity(),
		pVertexBuffer->GetUsage());

	Update(pVertexBuffer);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer*, UInt size, Buffer::UsageType
	usage)
	:
	mVertexSize(0)
{
	CreateBuffer(NULL, size, usage);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	free(mpBuffer);	// allocated using memalign, not using new
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::CreateBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
{
	mBufferSize = size;
	mpBuffer = memalign(32, mBufferSize);
	WIRE_ASSERT(mpBuffer);
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::CreateDeclaration(Renderer*, const VertexAttributes&
	rAttributes)
{
	mDeclaration.SetQuantity(0);
	VertexElement element;
	mVertexSize = 0;

	UInt channels = rAttributes.GetPositionChannels();
	if (channels > 0)
	{
		WIRE_ASSERT(channels == 3);
		element.Offset = mVertexSize;
		mVertexSize += channels * sizeof(Float);
		element.Attr = GX_VA_POS;
		element.CompCnt = GX_POS_XYZ;
		element.CompType = GX_F32;
		mDeclaration.Append(element);
	}

	channels = rAttributes.GetNormalChannels();
	if (channels > 0)
	{
		WIRE_ASSERT(channels == 3);
		element.Offset = mVertexSize;
		mVertexSize += channels * sizeof(Float);
		element.Attr = GX_VA_NRM;
		element.CompCnt = GX_NRM_XYZ;
		element.CompType = GX_F32;
		mDeclaration.Append(element);
	}

	for (UInt unit = 0; unit < rAttributes.GetColorChannelQuantity(); unit++)
	{
		if (rAttributes.GetColorChannels(unit) > 0)
		{
			element.Offset = mVertexSize;
			mVertexSize += sizeof(UInt);
			element.Attr = GX_VA_CLR0 + unit;
			element.CompCnt = GX_CLR_RGBA;
			element.CompType = GX_RGBA8;
			mDeclaration.Append(element);
		}
	}

	for (UInt unit = 0; unit < rAttributes.GetTCoordChannelQuantity(); unit++)
	{
		channels = rAttributes.GetTCoordChannels(unit);
		if (channels > 0)
		{
			WIRE_ASSERT(channels == 2);
			element.Offset = mVertexSize;
			mVertexSize += channels * sizeof(Float);
			element.Attr = GX_VA_TEX0 + unit;
			element.CompCnt = GX_TEX_ST;
			element.CompType = GX_F32;
			mDeclaration.Append(element);
		}
	}
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(mBufferSize == mVertexSize * pVertexBuffer->GetQuantity());
	WIRE_ASSERT(mVertexSize == pVertexBuffer->GetAttributes().
		GetChannelQuantity()*sizeof(Float));

	void* pData = Lock(Buffer::LM_WRITE_ONLY);
	System::Memcpy(pData, mBufferSize, pVertexBuffer->GetData(), mBufferSize);
	Unlock();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer)
{
	SetDeclaration(pRenderer);
	SetBuffer(pRenderer, mVertexSize);

	pRenderer->GetRendererData()->PdrVBuffer = this;
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer* pRenderer)
{
	pRenderer->GetRendererData()->PdrVBuffer = NULL;
}
