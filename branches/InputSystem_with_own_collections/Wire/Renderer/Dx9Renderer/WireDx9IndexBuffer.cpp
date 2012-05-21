// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9IndexBuffer.h"

#include "WireIndexBuffer.h"

using namespace Wire;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer*
	pIndexBuffer)
{
	UInt size = pIndexBuffer->GetQuantity() * sizeof(UShort);
	CreateBuffer(pRenderer, size, pIndexBuffer->GetUsage());

	Update(pIndexBuffer);
}

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
{
	CreateBuffer(pRenderer, size, usage);
}

//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer()
{
	mpBuffer->Release();
}

//----------------------------------------------------------------------------
void* PdrIndexBuffer::Lock(Buffer::LockingMode mode)
{
	void* pVideoMemory = 0;
	HRESULT hr;
	hr = mpBuffer->Lock(0, 0, &pVideoMemory,
		PdrRendererData::BUFFER_LOCKING[mode]);
	WIRE_ASSERT(SUCCEEDED(hr));
	return pVideoMemory;
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Unlock()
{
	HRESULT hr;
	hr = mpBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Update(const IndexBuffer* pIndexBuffer)
{
	WIRE_ASSERT((mBufferSize/sizeof(UShort)) == pIndexBuffer->GetQuantity());
	Buffer::LockingMode lockingMode = pIndexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;

	void* pBuffer = Lock(lockingMode);
	Copy(pIndexBuffer, pBuffer, 0);

	Unlock();
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Copy(const IndexBuffer* pIndexBuffer, void* pBuffer,
	UShort offset)
{
	const UShort* pIndices = pIndexBuffer->GetData();
	const UInt quantity = pIndexBuffer->GetQuantity();

	if (offset == 0)
	{
		const UInt size = pIndexBuffer->GetQuantity() * sizeof(UShort);
		System::Memcpy(pBuffer, size, pIndices, size);
		return;
	}

	UShort* pBuffer16 = reinterpret_cast<UShort*>(pBuffer);
	if ((quantity % 3) == 0)
	{
		for (UInt i = 0; i < quantity; i+=3)
		{
			pBuffer16[i] = pIndices[i] + offset;
			pBuffer16[i+1] = pIndices[i+1] + offset;
			pBuffer16[i+2] = pIndices[i+2] + offset;
		}
	}
	else
	{
		for (UInt i = 0; i < quantity; i++)
		{
			pBuffer16[i] = pIndices[i] + offset;
		}
	}
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::CreateBuffer(Renderer* pRenderer, UInt size, 
	Buffer::UsageType usage)
{
	mBufferSize = size;
	const DWORD d3dUsage = PdrRendererData::USAGES[usage];
	const D3DPOOL pool = PdrRendererData::POOLS[usage];
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->CreateIndexBuffer(mBufferSize, d3dUsage, D3DFMT_INDEX16,
		pool, &mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));
}
