// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
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

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	void* pVBData = Lock(Buffer::LM_WRITE_ONLY);

	const VertexAttributes& rAttr = pVertexBuffer->GetAttributes();
	Bool hasVertexColors = false;
	for (UInt unit = 0; unit < rAttr.GetColorChannelQuantity(); unit++)
	{
		if (rAttr.GetColorChannels(unit) > 0)
		{
			hasVertexColors = true;
		}
	}

	if (hasVertexColors)
	{
		Convert(pVertexBuffer, pVBData);
	}
	else
	{
		WIRE_ASSERT(mBufferSize == mVertexSize * pVertexBuffer->GetQuantity());
		WIRE_ASSERT(mVertexSize == rAttr.GetChannelQuantity()*sizeof(Float));
		System::Memcpy(pVBData, mBufferSize, pVertexBuffer->GetData(),
			mBufferSize);
	}

	Unlock();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Convert(const VertexBuffer* pSrc, void* pVoid)
{
	Float* pDst = reinterpret_cast<Float*>(pVoid);

	const VertexAttributes& rIAttr = pSrc->GetAttributes();

	for (UInt i = 0; i < pSrc->GetQuantity(); i++)
	{
		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* const pPosition = pSrc->GetPosition(i);
			for (UInt k = 0; k < rIAttr.GetPositionChannels(); k++)
			{
				*pDst++ = pPosition[k];
			}
		}

		if (rIAttr.GetNormalChannels() > 0)
		{
			const Float* const pNormal = pSrc->GetNormal(i);
			for (UInt k = 0; k < rIAttr.GetNormalChannels(); k++)
			{
				*pDst++ = pNormal[k];
			}
		}

		UInt colorChannelQuantity = rIAttr.GetColorChannelQuantity();
		for (UInt unit = 0; unit < colorChannelQuantity; unit++)
		{
			if (rIAttr.GetColorChannels(unit) > 0)
			{
				const Float* const pColor = pSrc->GetColor(i, unit);
				UInt color = 0xFFFFFFFF;
				for (UInt k = 0; k < rIAttr.GetColorChannels(unit); k++)
				{
					color = color << 8;
					color |= static_cast<UChar>(pColor[k] * 255.0F);
				}

				if (rIAttr.GetColorChannels(unit) == 3)
				{
					color = color << 8;
					color |= 0xFF;
				}

				UInt* pColorDst = reinterpret_cast<UInt*>(pDst);
				*pColorDst++ = color;
				pDst = reinterpret_cast<Float*>(pColorDst);
			}
		}

		UInt tCoordChannelQuantity = rIAttr.GetTCoordChannelQuantity();
		for (UInt unit = 0; unit < tCoordChannelQuantity; unit++)
		{
			UInt channels = rIAttr.GetTCoordChannels(unit);
			if (channels > 0)
			{
				const Float* const pTCoords = pSrc->GetTCoord(i, unit);
				for (UInt k = 0; k < channels; k++)
				{
					*pDst++ = pTCoords[k];
				}
			}
		}
	}
}
