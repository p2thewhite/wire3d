// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
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
	WIRE_ASSERT((mBufferSize/mIndexSize) == pIndexBuffer->GetQuantity());
	Buffer::LockingMode lockingMode = pIndexBuffer->GetUsage() ==
		Buffer::UT_STATIC ? Buffer::LM_READ_WRITE : Buffer::LM_WRITE_ONLY;

	void* pBuffer = Lock(lockingMode);
	Copy(pIndexBuffer, pBuffer, 0);

	Unlock();
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Copy(const IndexBuffer* pIndexBuffer, void* pBuffer,
	UInt offset)
{
	const UInt* pIndices = pIndexBuffer->GetData();
	const UInt quantity = pIndexBuffer->GetQuantity();

	if (offset == 0)
	{
		if (mIndexSize == sizeof(UInt))
		{
			System::Memcpy(pBuffer, mBufferSize, pIndices, mBufferSize);
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
	}
	else
	{
		WIRE_ASSERT(mIndexSize == sizeof(UShort));
		UShort* pBuffer16 = reinterpret_cast<UShort*>(pBuffer);

		if ((quantity % 3) == 0)
		{
			for (UInt i = 0; i < quantity; i+=3)
			{
				pBuffer16[i] = static_cast<UShort>(pIndices[i] + offset);
				pBuffer16[i+1] = static_cast<UShort>(pIndices[i+1] + offset);
				pBuffer16[i+2] = static_cast<UShort>(pIndices[i+2] + offset);
			}
		}
		else
		{
			for (UInt i = 0; i < quantity; i++)
			{
				pBuffer16[i] = static_cast<UShort>(pIndices[i] + offset);
			}
		}
	}
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::CreateBuffer(Renderer* pRenderer, UInt size, 
	Buffer::UsageType usage)
{
	mBufferSize = size;
	D3DFORMAT format = (mIndexSize == sizeof(UShort)) ? D3DFMT_INDEX16 :
		D3DFMT_INDEX32;

	const DWORD d3dUsage = PdrRendererData::USAGES[usage];
	const D3DPOOL pool = PdrRendererData::POOLS[usage];
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->CreateIndexBuffer(mBufferSize, d3dUsage, format, pool,
		&mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));
}
