// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9VertexBuffer.h"

#include "WireDx9RendererData.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"
#include <d3d9.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer*
	pVertexBuffer)
	:
	mVertexSize(0)
{
	TArray<D3DVERTEXELEMENT9> elements(8);
	D3DVERTEXELEMENT9 element;
	element.Stream = 0;
	element.Method = D3DDECLMETHOD_DEFAULT;

	UInt channels = 0;

	const VertexAttributes& rIAttr = pVertexBuffer->GetAttributes();

	if (rIAttr.GetPositionChannels() > 0)
	{
		channels = rIAttr.GetPositionChannels();
		element.Offset = static_cast<WORD>(mVertexSize);
		mVertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_POSITION;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	if (rIAttr.GetNormalChannels() > 0)
	{
		channels = rIAttr.GetNormalChannels();
		element.Offset = static_cast<WORD>(mVertexSize);
		mVertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_NORMAL;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
			element.Offset = static_cast<WORD>(mVertexSize);
			mVertexSize += sizeof(DWORD);
			element.Type = D3DDECLTYPE_D3DCOLOR;
			element.Usage = D3DDECLUSAGE_COLOR;
			element.UsageIndex = static_cast<BYTE>(unit);
			elements.Append(element);
		}
	}

	for (UInt unit = 0; unit < rIAttr.GetTCoordChannelQuantity(); unit++)
	{
		channels = rIAttr.GetTCoordChannels(unit);
		if (channels > 0)
		{
			element.Offset = static_cast<WORD>(mVertexSize);
			mVertexSize += channels * sizeof(Float);
			element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels-1);
			element.Usage = D3DDECLUSAGE_TEXCOORD;
			element.UsageIndex = static_cast<BYTE>(unit);
			elements.Append(element);
		}
	}

	WIRE_ASSERT(channels > 0);

	D3DVERTEXELEMENT9 sentinel = D3DDECL_END();
	elements.Append(sentinel);

	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->CreateVertexDeclaration(&elements[0], &mpDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Create the vertex buffer.
	UInt vbSize = mVertexSize * pVertexBuffer->GetVertexQuantity();
	const DWORD usage = PdrRendererData::sUsages[pVertexBuffer->GetUsage()];
	const D3DPOOL pool = PdrRendererData::sPools[pVertexBuffer->GetUsage()];
	hr = rDevice->CreateVertexBuffer(vbSize, usage, 0, pool, &mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Copy the vertex buffer data from system memory to video memory.
	Update(pVertexBuffer);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	mpBuffer->Release();
	mpDeclaration->Release();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer, const VertexBuffer*)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->SetStreamSource(0, mpBuffer, 0, mVertexSize);
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetVertexDeclaration(mpDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer* pRenderer)
{
	HRESULT hr;
	hr = pRenderer->GetRendererData()->D3DDevice->SetStreamSource(
		0, NULL, 0, 0);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void* PdrVertexBuffer::Lock(Buffer::LockingMode mode)
{
	void* pVideoMemory = 0;
	HRESULT hr;
	hr = mpBuffer->Lock(0, 0, &pVideoMemory,
		PdrRendererData::sBufferLocking[mode]);
	WIRE_ASSERT(SUCCEEDED(hr));
	return pVideoMemory;
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Unlock()
{
	HRESULT hr;
	hr = mpBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	Buffer::LockingMode lockingMode = pVertexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY; 
	Float* pVBData = reinterpret_cast<Float*>(Lock(lockingMode));
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
		UInt size =  mVertexSize * pVertexBuffer->GetVertexQuantity();
 		WIRE_ASSERT(mVertexSize == rIAttr.GetChannelQuantity()*sizeof(Float));
		System::Memcpy(pVBData, size, pVertexBuffer->GetData(), size);
	}

	Unlock();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Convert(const VertexBuffer* pSrc, Float* pDst)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
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
				D3DCOLOR color = 0xFFFFFFFF;
				for (UInt k = 0; k < rIAttr.GetColorChannels(unit); k++)
				{
					color = color << 8;
					color |= static_cast<UChar>(pColor[k] * 255.0F);
				}

				if (rIAttr.GetColorChannels(unit) == 4)
				{
					UChar alpha = static_cast<UChar>(color);
					color = color >> 8;
					color |= alpha << 24;
				}

				DWORD* pColorDst = reinterpret_cast<DWORD*>(pDst);
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
