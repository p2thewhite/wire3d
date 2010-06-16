#include "WireDx9VertexBuffer.h"

#include "WireDx9RendererData.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"
#include <d3d9.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer* pRenderer, const VertexBuffer*
	pVertexBuffer)
{
	PdrRendererData& rData = *(pRenderer->GetRendererData());

	TArray<D3DVERTEXELEMENT9> elements(8);
	D3DVERTEXELEMENT9 element;
	element.Stream = 0;
	element.Method = D3DDECLMETHOD_DEFAULT;

	UInt vertexSize = 0;
	UInt channels = 0;

	const VertexAttributes& rIAttr = pVertexBuffer->GetAttributes();

	if (rIAttr.GetPositionChannels() > 0)
	{
		channels = rIAttr.GetPositionChannels();
		element.Offset = static_cast<WORD>(vertexSize);
		vertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_POSITION;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	for (UInt unit = 0; unit < rIAttr.GetColorChannelQuantity(); unit++)
	{
		if (rIAttr.GetColorChannels(unit) > 0)
		{
			element.Offset = static_cast<WORD>(vertexSize);
			vertexSize += sizeof(DWORD);
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
			element.Offset = static_cast<WORD>(vertexSize);
			vertexSize += channels * sizeof(Float);
			element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels-1);
			element.Usage = D3DDECLUSAGE_TEXCOORD;
			element.UsageIndex = static_cast<BYTE>(unit);
			elements.Append(element);
		}
	}

	WIRE_ASSERT(channels > 0);

	D3DVERTEXELEMENT9 sentinel = D3DDECL_END();
	elements.Append(sentinel);

	IDirect3DDevice9*& rDevice = rData.mpD3DDevice;
	HRESULT hr;
	hr = rDevice->CreateVertexDeclaration(&elements[0], &mpVertexDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Create the vertex buffer.
	UInt vbSize = vertexSize * pVertexBuffer->GetVertexQuantity();
	hr = rDevice->CreateVertexBuffer(vbSize, 0, 0, D3DPOOL_MANAGED,
		&mpVertexBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(hr));

	// Copy the vertex buffer data from system memory to video memory.
	Float* pVBData;
	hr = mpVertexBuffer->Lock(0, vbSize, reinterpret_cast<void**>(&pVBData),
		0);
	WIRE_ASSERT(SUCCEEDED(hr));

	rData.Convert(pVertexBuffer, pVBData);

	hr = mpVertexBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(hr));

	mVertexSize = vertexSize;
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	mpVertexBuffer->Release();
	mpVertexDeclaration->Release();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer, const VertexBuffer*)
{
	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->mpD3DDevice;
	HRESULT hr;
	hr = rDevice->SetStreamSource(0, mpVertexBuffer, 0, mVertexSize);
	WIRE_ASSERT(SUCCEEDED(hr));
	hr = rDevice->SetVertexDeclaration(mpVertexDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));

}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer*)
{
	// Nothing to do
}
