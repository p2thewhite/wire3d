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
	const DWORD usage = PdrRendererData::sUsages[pIndexBuffer->GetUsage()];
	const D3DPOOL pool = PdrRendererData::sPools[pIndexBuffer->GetUsage()];
	HRESULT hr;
	hr = rData.D3DDevice->CreateIndexBuffer(indexBufferSize, usage, format,
		pool, &mpBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Copy the index buffer data from system memory to video memory.
	Char* pBuffer = reinterpret_cast<Char*>(Lock(Buffer::LM_WRITE_ONLY));	
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
void PdrIndexBuffer::Enable(Renderer* pRenderer, const IndexBuffer*)
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
		PdrRendererData::sBufferLocking[mode]);
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
