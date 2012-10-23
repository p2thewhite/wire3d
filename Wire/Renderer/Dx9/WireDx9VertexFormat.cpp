// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireDx9VertexFormat.h"

#include "WireRenderer.h"

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexFormat::PdrVertexFormat(Renderer* pRenderer, const VertexAttributes&
	rAttributes)
	:
	mpDeclaration(NULL)
{
	TArray<D3DVERTEXELEMENT9> elements(8);
	D3DVERTEXELEMENT9 element;
	element.Stream = 0;
	element.Method = D3DDECLMETHOD_DEFAULT;

	UInt channels = 0;
	UInt vertexSize = 0;

	if (rAttributes.GetPositionChannels() > 0)
	{
		channels = rAttributes.GetPositionChannels();
		element.Offset = static_cast<WORD>(vertexSize);
		vertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_POSITION;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	if (rAttributes.GetNormalChannels() > 0)
	{
		channels = rAttributes.GetNormalChannels();
		element.Offset = static_cast<WORD>(vertexSize);
		vertexSize += channels * sizeof(Float);
		element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels - 1);
		element.Usage = D3DDECLUSAGE_NORMAL;
		element.UsageIndex = 0;
		elements.Append(element);
	}

	for (UInt unit = 0; unit < rAttributes.GetColorChannelQuantity(); unit++)
	{
		if (rAttributes.GetColorChannels(unit) > 0)
		{
			element.Offset = static_cast<WORD>(vertexSize);
			vertexSize += sizeof(DWORD);
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
			element.Offset = static_cast<WORD>(vertexSize);
			vertexSize += channels * sizeof(Float);
			element.Type = static_cast<BYTE>(D3DDECLTYPE_FLOAT1 + channels-1);
			element.Usage = D3DDECLUSAGE_TEXCOORD;
			element.UsageIndex = static_cast<BYTE>(unit);
			elements.Append(element);
		}
	}

	WIRE_ASSERT(channels > 0);
	WIRE_ASSERT(vertexSize == rAttributes.GetVertexSize());

	D3DVERTEXELEMENT9 sentinel = D3DDECL_END();
	elements.Append(sentinel);

	IDirect3DDevice9*& rDevice = pRenderer->GetRendererData()->D3DDevice;
	HRESULT hr;
	hr = rDevice->CreateVertexDeclaration(&elements[0], &mpDeclaration);
	WIRE_ASSERT(SUCCEEDED(hr));
	WIRE_ASSERT(mpDeclaration);
}

//----------------------------------------------------------------------------
PdrVertexFormat::~PdrVertexFormat()
{
	if (mpDeclaration)
	{
		mpDeclaration->Release();
	}
}
