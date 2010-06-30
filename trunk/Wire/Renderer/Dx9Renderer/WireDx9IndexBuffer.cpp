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
	HRESULT hr;
	hr = rData.D3DDevice->CreateIndexBuffer(indexBufferSize,
		D3DUSAGE_WRITEONLY, format, D3DPOOL_MANAGED, &mpIndexBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Copy the index buffer data from system memory to video memory.
	Char* pLockedIBuffer;
	hr = mpIndexBuffer->Lock(0, indexBufferSize, reinterpret_cast<void**>(
		&pLockedIBuffer), 0);
	WIRE_ASSERT(SUCCEEDED(hr));

	if (rData.Supports32BitIndices)
	{
		System::Memcpy(pLockedIBuffer, indexBufferSize, pIndices,
			indexBufferSize);
	}
	else
	{
		UShort* pBuffer = reinterpret_cast<UShort*>(pLockedIBuffer);

		if ((quantity % 3) == 0)
		{
			for (UInt i = 0; i < quantity; i+=3)
			{
				pBuffer[i] = static_cast<UShort>(pIndices[i]);
				pBuffer[i+1] = static_cast<UShort>(pIndices[i+1]);
				pBuffer[i+2] = static_cast<UShort>(pIndices[i+2]);
			}
		}
		else
		{
			for (UInt i = 0; i < quantity; i++)
			{
				pBuffer[i] = static_cast<UShort>(pIndices[i]);
			}
		}
	}

	hr = mpIndexBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer()
{
	mpIndexBuffer->Release();
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable(Renderer* pRenderer, const IndexBuffer*)
{
	HRESULT hr;
	hr = pRenderer->GetRendererData()->D3DDevice->SetIndices(mpIndexBuffer);
	WIRE_ASSERT(SUCCEEDED(hr));
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable(Renderer*)
{
	// Nothing to do
}
