// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9VertexBuffer.h"

#include "WireVertexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer*
	pVertexBuffer)
	:
	mpDeclaration(NULL),
	mVertexSize(0)
{
	CreateDeclaration(pRenderer, pVertexBuffer->GetAttributes());
	CreateBuffer(pRenderer, mVertexSize * pVertexBuffer->GetQuantity(),
		pVertexBuffer->GetUsage());

	// Copy the vertex buffer data from system memory to video memory.
	Update(pVertexBuffer);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
	:
	mpDeclaration(NULL),
	mVertexSize(0)
{
	CreateBuffer(pRenderer, size, usage);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	mpBuffer->Release();
	if (mpDeclaration)
	{
		mpDeclaration->Release();
	}
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::CreateBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
{
	mBufferSize = size;
	const DWORD d3dUsage = PdrRendererData::USAGES[usage];
	const D3DPOOL pool = PdrRendererData::POOLS[usage];
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->CreateVertexBuffer(mBufferSize, d3dUsage, 0, pool,
		&mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::CreateDeclaration(Renderer* pRenderer, const
	VertexAttributes& rAttributes)
{
	if (mpDeclaration)
	{
		mpDeclaration->Release();
	}

	mVertexSize = 0;

	TArray<D3DVERTEXELEMENT9> elements(8);
	D3DVERTEXELEMENT9 element;
	element.Stream = 0;
	element.Method = D3DDECLMETHOD_DEFAULT;

	UInt channels = 0;

	if (rAttributes.GetPositionChannels() > 0)
	{
		channels = rAttributes.GetPositionChannels();
		element.Offset = static_cast<WORD>(mVertexSize);
		mVertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_POSITION;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	if (rAttributes.GetNormalChannels() > 0)
	{
		channels = rAttributes.GetNormalChannels();
		element.Offset = static_cast<WORD>(mVertexSize);
		mVertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_NORMAL;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	for (UInt unit = 0; unit < rAttributes.GetColorChannelQuantity(); unit++)
	{
		if (rAttributes.GetColorChannels(unit) > 0)
		{
			element.Offset = static_cast<WORD>(mVertexSize);
			mVertexSize += sizeof(DWORD);
			element.Type = D3DDECLTYPE_D3DCOLOR;
			element.Usage = D3DDECLUSAGE_COLOR;
			element.UsageIndex = static_cast<BYTE>(unit);
			elements.Append(element);
		}
	}

	for (UInt unit = 0; unit < rAttributes.GetTCoordChannelQuantity(); unit++)
	{
		channels = rAttributes.GetTCoordChannels(unit);
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
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer)
{
	SetBuffer(pRenderer, mVertexSize);
	SetDeclaration(pRenderer);
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
	void* pBuffer = 0;
	HRESULT hr;
	hr = mpBuffer->Lock(0, 0, &pBuffer,
		PdrRendererData::BUFFER_LOCKING[mode]);
	WIRE_ASSERT(SUCCEEDED(hr));
	return pBuffer;
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	WIRE_ASSERT(mVertexSize > 0);
	WIRE_ASSERT((mBufferSize/mVertexSize) == pVertexBuffer->GetQuantity());

	Buffer::LockingMode lockingMode = pVertexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;
	void* pVBData = Lock(lockingMode);

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

	Unlock();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Copy(const VertexBuffer* pVertexBuffer, void* pBuffer,
	const Transformation& rTransformation)
{
	const VertexAttributes& rIAttr = pVertexBuffer->GetAttributes();
	WIRE_ASSERT(mVertexSize == rIAttr.GetChannelQuantity()*sizeof(Float));
	Bool hasVertexColors = false;
	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
			hasVertexColors = true;
		}
	}

	if (rTransformation.IsIdentity())
	{
		if (hasVertexColors)
		{
			Convert(pVertexBuffer, pBuffer);
		}
		else
		{
			UInt size =  mVertexSize * pVertexBuffer->GetQuantity();
			System::Memcpy(pBuffer, size, pVertexBuffer->GetData(), size);
		}
	}
	else
	{
		Convert(pVertexBuffer, pBuffer, rTransformation);
	}
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

//----------------------------------------------------------------------------
void PdrVertexBuffer::Convert(const VertexBuffer* pSrc, void* pVoid, const
	Transformation& rTransformation)
{
	WIRE_ASSERT(!rTransformation.IsIdentity() /* redundant transform */);
	Float* pDst = reinterpret_cast<Float*>(pVoid);
	const VertexAttributes& rIAttr = pSrc->GetAttributes();
	const Vector3F& translate = rTransformation.GetTranslate();
	const Matrix34F& rotate = rTransformation.GetMatrix();

	for (UInt i = 0; i < pSrc->GetQuantity(); i++)
	{
		if (rIAttr.GetPositionChannels() == 3)
		{
			const Float* const pPosition = pSrc->GetPosition(i);
			Vector3F v(pPosition[0], pPosition[1], pPosition[2]);

			if (rTransformation.IsRSMatrix())
			{
				const Vector3F& scale = rTransformation.GetScale();
				v = Vector3F(scale.X() * v.X(), scale.Y() * v.Y(),
					scale.Z() * v.Z());
			}

			v = rotate * v + translate;

			*pDst++ = v.X();
			*pDst++ = v.Y();
			*pDst++ = v.Z();
		}
		else
		{
			WIRE_ASSERT(false /* implement transform for non 3d pos? */);
			const Float* const pPosition = pSrc->GetPosition(i);
			for (UInt k = 0; k < rIAttr.GetPositionChannels(); k++)
			{
				*pDst++ = pPosition[k];
			}
		}

		if (rIAttr.GetNormalChannels() == 3)
		{
			const Float* const pNormal = pSrc->GetNormal(i);
			Vector3F n(pNormal[0], pNormal[1], pNormal[2]);

			if (rTransformation.IsRSMatrix())
			{
				const Vector3F& scale = rTransformation.GetScale();
				n = Vector3F(scale.X() * n.X(), scale.Y() * n.Y(),
					scale.Z() * n.Z());
			}

			n = rotate * n;

			*pDst++ = n.X();
			*pDst++ = n.Y();
			*pDst++ = n.Z();
		}
		else if (rIAttr.GetNormalChannels() > 0)
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
