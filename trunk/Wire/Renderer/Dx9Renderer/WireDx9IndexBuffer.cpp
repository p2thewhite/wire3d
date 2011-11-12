// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9IndexBuffer.h"

#include "WireDx9RendererData.h"
#include "WireRenderer.h"
#include "WireIndexBuffer.h"
#include <d3d9.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer*
	pIndexBuffer)
	:
	mIndexSize(sizeof(UInt))
{
	PdrRendererData& rData = *(pRenderer->GetRendererData());
	if (!rData.Supports32BitIndices)
	{
		mIndexSize = sizeof(UShort);
	}

	UInt size = pIndexBuffer->GetQuantity() * mIndexSize;
	CreateBuffer(pRenderer, size, pIndexBuffer->GetUsage());

	Update(pIndexBuffer);

	Renderer::Statistics* pStatistics = const_cast<Renderer::Statistics*>(
		Renderer::GetStatistics());
	WIRE_ASSERT(pStatistics);
	pStatistics->IBOCount++;
	pStatistics->IBOTotalSize += mIBOSize;
}

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
	:
	mIndexSize(sizeof(UInt))
{
	PdrRendererData& rData = *(pRenderer->GetRendererData());
	if (!rData.Supports32BitIndices)
	{
		mIndexSize = sizeof(UShort);
	}

	CreateBuffer(pRenderer, size, usage);

	Renderer::Statistics* pStatistics = const_cast<Renderer::Statistics*>(
		Renderer::GetStatistics());
	WIRE_ASSERT(pStatistics);
	pStatistics->IBOCount++;
	pStatistics->IBOTotalSize += mIBOSize;
}

//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer()
{
	mpBuffer->Release();

	Renderer::Statistics* pStatistics = const_cast<Renderer::Statistics*>(
		Renderer::GetStatistics());
	WIRE_ASSERT(pStatistics);
	pStatistics->IBOCount--;
	pStatistics->IBOTotalSize -= mIBOSize;
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable(Renderer* pRenderer)
{
	HRESULT hr;
	hr = pRenderer->GetRendererData()->D3DDevice->SetIndices(mpBuffer);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable(Renderer* pRenderer)
{
	HRESULT hr;
	hr = pRenderer->GetRendererData()->D3DDevice->SetIndices(NULL);
	WIRE_ASSERT(SUCCEEDED(hr));
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
	Buffer::LockingMode lockingMode = pIndexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;
	const UInt quantity = pIndexBuffer->GetQuantity();

	// Copy the index buffer data from system memory to video memory.
	Char* pBuffer = reinterpret_cast<Char*>(Lock(lockingMode));
	const UInt* pIndices = pIndexBuffer->GetData();

	if (mIndexSize == sizeof(UInt))
	{
		System::Memcpy(pBuffer, mIBOSize, pIndices, mIBOSize);
	}
	else
	{
		WIRE_ASSERT(mIndexSize == sizeof(UShort));
		UShort* pBuffer16 = reinterpret_cast<UShort*>(pBuffer);

		if ((quantity % 3) == 0)
		{
			for (UInt i = 0; i < quantity; i+=3)
			{
				pBuffer16[i] = static_cast<UShort>(pIndices[i]);
				pBuffer16[i+1] = static_cast<UShort>(pIndices[i+1]);
				pBuffer16[i+2] = static_cast<UShort>(pIndices[i+2]);
			}
		}
		else
		{
			for (UInt i = 0; i < quantity; i++)
			{
				pBuffer16[i] = static_cast<UShort>(pIndices[i]);
			}
		}
	}

	Unlock();
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::CreateBuffer(Renderer* pRenderer, UInt size, 
	Buffer::UsageType usage)
{
	mIBOSize = size;
	D3DFORMAT format = (mIndexSize == sizeof(UShort)) ? D3DFMT_INDEX16 :
		D3DFMT_INDEX32;

	const DWORD d3dUsage = PdrRendererData::USAGES[usage];
	const D3DPOOL pool = PdrRendererData::POOLS[usage];
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->CreateIndexBuffer(mIBOSize, d3dUsage, format, pool,
		&mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));
}
