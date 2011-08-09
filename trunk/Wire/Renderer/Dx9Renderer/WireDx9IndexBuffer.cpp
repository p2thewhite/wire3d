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
{
	UInt quantity = pIndexBuffer->GetIndexQuantity();
	const UInt* pIndices = pIndexBuffer->GetData();
	UInt indexBufferSize = quantity * sizeof(UInt);
	D3DFORMAT format = D3DFMT_INDEX32;

	PdrRendererData& rData = *(pRenderer->GetRendererData());

	if (!rData.Supports32BitIndices)
	{
		indexBufferSize = quantity * sizeof(UShort);
		format = D3DFMT_INDEX16;
	}

	// Create the index buffer.
	const DWORD usage = PdrRendererData::USAGES[pIndexBuffer->GetUsage()];
	const D3DPOOL pool = PdrRendererData::POOLS[pIndexBuffer->GetUsage()];
	HRESULT hr;
	hr = rData.D3DDevice->CreateIndexBuffer(indexBufferSize, usage, format,
		pool, &mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));

	Buffer::LockingMode lockingMode = pIndexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;

	// Copy the index buffer data from system memory to video memory.
	Char* pBuffer = reinterpret_cast<Char*>(Lock(lockingMode));
	if (rData.Supports32BitIndices)
	{
		System::Memcpy(pBuffer, indexBufferSize, pIndices,
			indexBufferSize);
	}
	else
	{
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
PdrIndexBuffer::~PdrIndexBuffer()
{
	mpBuffer->Release();
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
