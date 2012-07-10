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
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	Update(pVertexBuffer, pVertexBuffer->GetQuantity());
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer, UInt count,
	UInt offset)
{
	WIRE_ASSERT(mVertexSize > 0);
	WIRE_ASSERT(mBufferSize == (mVertexSize * pVertexBuffer->GetQuantity()));
	WIRE_ASSERT(mVertexSize == pVertexBuffer->GetAttributes().
		GetChannelQuantity()*sizeof(Float));

	Buffer::LockingMode lockingMode = pVertexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;
	UChar* pBuffer = reinterpret_cast<UChar*>(Lock(lockingMode)) +
		offset * mVertexSize;

	size_t size = count * mVertexSize;
	const UChar* pDst = reinterpret_cast<const UChar*>(pVertexBuffer->
		GetData()) + offset * mVertexSize;
	System::Memcpy(pBuffer, size, pDst, size);

	Unlock();
}
