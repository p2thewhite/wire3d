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
	VertexElement element;

	const VertexAttributes& rIAttr = pVertexBuffer->GetAttributes();
	UInt channels = rIAttr.GetPositionChannels();
	if (channels > 0)
	{
		element.Data = reinterpret_cast<void*>(mVertexSize);
		mVertexSize += channels * sizeof(Float);
		element.Attr = GX_VA_POS;
		element.CompCnt = GX_POS_XYZ;
		element.CompType = GX_F32;
		mElements.Append(element);
	}

	channels = rIAttr.GetNormalChannels();
	if (channels > 0)
	{
		element.Data = reinterpret_cast<void*>(mVertexSize);
		mVertexSize += channels * sizeof(Float);
		element.Attr = GX_VA_NRM;
		element.CompCnt = GX_NRM_XYZ;
		element.CompType = GX_F32;
		mElements.Append(element);
	}

	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
			element.Data = reinterpret_cast<void*>(mVertexSize);
			mVertexSize += sizeof(UInt);
			element.Attr = GX_VA_CLR0 + unit;
			element.CompCnt = GX_CLR_RGBA;
			element.CompType = GX_RGBA8;
			mElements.Append(element);
		}
	}

	for (UInt unit = 0; unit < rIAttr.GetTCoordChannelQuantity(); unit++)
	{
		channels = rIAttr.GetTCoordChannels(unit);
		if (channels > 0)
		{
			element.Data = reinterpret_cast<void*>(mVertexSize);
			mVertexSize += channels * sizeof(Float);
			element.Attr = GX_VA_TEX0 + unit;
			element.CompCnt = GX_TEX_ST;
			element.CompType = GX_F32;
			mElements.Append(element);
		}
	}

	UInt vbSize = mVertexSize * pVertexBuffer->GetQuantity();
	mpData = memalign(32, vbSize);

	for (UInt i = 0; i < mElements.GetQuantity(); i++)
	{
		UInt offset = reinterpret_cast<UInt>(mElements[i].Data);
		offset += reinterpret_cast<UInt>(mpData);
		mElements[i].Data = reinterpret_cast<void*>(offset);
	}

	Update(pVertexBuffer);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	free(mpData);	// allocated using memalign, not using new
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer)
{
	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();

	for (UInt i = 0; i < mElements.GetQuantity(); i++)
	{
		GXSetVtxDesc(mElements[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, mElements[i].Attr, mElements[i].CompCnt,
			mElements[i].CompType, 0);
		GXSetArray(mElements[i].Attr, mElements[i].Data, mVertexSize);
	}

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
	Float* pVBData = static_cast<Float*>(mpData);

	const VertexAttributes& rIAttr = pVertexBuffer->GetAttributes();
	Bool hasVertexColors = false;
	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
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
		UInt size =  mVertexSize * pVertexBuffer->GetQuantity();
		WIRE_ASSERT(mVertexSize == rIAttr.GetChannelQuantity()*sizeof(Float));
		System::Memcpy(pVBData, size, pVertexBuffer->GetData(), size);
	}

	DCStoreRange(mpData, mVertexSize * pVertexBuffer->GetQuantity());
	GXInvalidateVtxCache();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Convert(const VertexBuffer* pSrc, Float* pDst)
{
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
