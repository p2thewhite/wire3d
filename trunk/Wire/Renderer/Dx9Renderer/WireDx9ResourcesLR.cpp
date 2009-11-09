#include "WireDx9Renderer.h"

#include "WireDx9Resources.h"
#include <d3dx9.h>

using namespace Wire;

//----------------------------------------------------------------------------
void Dx9Renderer::OnLoadIBuffer(ResourceIdentifier*& rID,
	IndexBuffer* pIBuffer)
{
	// The index buffer is encountered the first time.
	IBufferID* pResource = WIRE_NEW IBufferID;
	rID = pResource;

	UInt quantity = pIBuffer->GetIndexQuantity();
	UInt* pIndices = pIBuffer->GetData();
	UInt indexBufferSize = quantity * sizeof(UInt);

	// Create the index buffer.
	D3DFORMAT format = mSupports32BitIndices ? D3DFMT_INDEX32 :
		D3DFMT_INDEX16;
	IDirect3DIndexBuffer9* pD3DIBuffer;
	msResult = mpDevice->CreateIndexBuffer(indexBufferSize,
		D3DUSAGE_WRITEONLY, format, D3DPOOL_MANAGED, &pD3DIBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Copy the index buffer data from system memory to video memory.
	Char* pLockedIBuffer;
	msResult = pD3DIBuffer->Lock(0, indexBufferSize,
		reinterpret_cast<void**>(&pLockedIBuffer), 0);
	WIRE_ASSERT(SUCCEEDED(msResult));

	if (mSupports32BitIndices)
	{
		System::Memcpy(pLockedIBuffer, indexBufferSize, pIndices,
			indexBufferSize);
	}
	else
	{
		if ((quantity % 3) == 0)
		{
			for (UInt i = 0; i < quantity; i+=3)
			{
				reinterpret_cast<UShort*>(pLockedIBuffer)[i] =
					static_cast<UShort>(pIndices[i]);
				reinterpret_cast<UShort*>(pLockedIBuffer)[i+1] =
					static_cast<UShort>(pIndices[i+1]);
				reinterpret_cast<UShort*>(pLockedIBuffer)[i+2] =
					static_cast<UShort>(pIndices[i+2]);
			}
		}
		else
		{
			for (UInt i = 0; i < quantity; i++)
			{
				reinterpret_cast<UShort*>(pLockedIBuffer)[i] =
					static_cast<UShort>(pIndices[i]);
			}
		}
	}

	msResult = pD3DIBuffer->Unlock();
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Generate the binding information and save it.
	pResource->ID = pD3DIBuffer;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnReleaseIBuffer(ResourceIdentifier* pID)
{
	IBufferID* pResource = static_cast<IBufferID*>(pID);
	pResource->ID->Release();
	WIRE_DELETE pResource;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnLoadVBuffer(ResourceIdentifier*& rID,
	VertexBuffer* pVBuffer)
{
 	VBufferID* pResource = WIRE_NEW VBufferID;
 	rID = pResource;
 
 	TArray<D3DVERTEXELEMENT9> elements;
 	D3DVERTEXELEMENT9 element;
 	element.Stream = 0;
 	element.Method = D3DDECLMETHOD_DEFAULT;
 
 	UInt vertexSize = 0;
	UInt channels = 0;

	const VertexAttributes& rIAttr = pVBuffer->GetAttributes();
 
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
			element.UsageIndex = (BYTE)unit;
			elements.Append(element);
 		}
 	}

	WIRE_ASSERT(channels > 0);

 	D3DVERTEXELEMENT9 sentinel = D3DDECL_END();
 	elements.Append(sentinel);
 
 	msResult = mpDevice->CreateVertexDeclaration(&elements[0],
		&pResource->Declaration);
 	WIRE_ASSERT(SUCCEEDED(msResult));
 
 	// Copy the vertex buffer data to an array. NOTE: The output channels
 	// will be smaller than vertexSize*vertexQuantity for pVBuffer whenever
 	// the vertex buffer attributes have colors. This is because a ColorRGBA
 	// value is 4 floats but is packed into 1 float.
//	Float* afCompatible = 0;  // allocated by BuildCompatibleArray
// 	pVBuffer->BuildCompatibleArray(rIAttr,true,channels,afCompatible);
 
 	// Create the vertex buffer.
	UInt vbSize = (static_cast<UInt>(channels * sizeof(Float)) + sizeof(DWORD)) * pVBuffer->GetVertexQuantity();
// 	UInt vbSize = static_cast<UInt>(channels * sizeof(Float));
 	IDirect3DVertexBuffer9* pD3DVBuffer;
 	msResult = mpDevice->CreateVertexBuffer(vbSize, 0, 0, D3DPOOL_MANAGED,
 		&pD3DVBuffer, NULL);
 	WIRE_ASSERT(SUCCEEDED(msResult));
 
 	// Copy the vertex buffer data from system memory to video memory.
 	Float* pVBData;
 	msResult = pD3DVBuffer->Lock(0, vbSize, reinterpret_cast<void**>(
		&pVBData), 0);
 	WIRE_ASSERT(SUCCEEDED(msResult));





	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 position; // The position
		D3DCOLOR    color;    // The color
	};

	CUSTOMVERTEX vertices[1000];
	for (UInt i = 0; i < mpGeometry->VBuffer->GetVertexQuantity(); i++)
	{
		Vector3F& rVec = mpGeometry->VBuffer->Position3(i);
		vertices[i].position = D3DXVECTOR3(rVec.X(), rVec.Y(), rVec.Z());
		ColorRGB& rCol = mpGeometry->VBuffer->Color3(i);
		UInt r = static_cast<UInt>(rCol.R() * 255.0F);
		UInt g = static_cast<UInt>(rCol.G() * 255.0F);
		UInt b = static_cast<UInt>(rCol.B() * 255.0F);
		D3DCOLOR col = (0xff << 24) + (r << 16) + (g << 8) + b;
		vertices[i].color = col;
	}


	System::Memcpy(pVBData, vbSize, vertices, vbSize);

//	System::Memcpy(pVBData, vbSize, afCompatible, vbSize);
	msResult = pD3DVBuffer->Unlock();
 	WIRE_ASSERT(SUCCEEDED(msResult));
 
// 	WIRE_DELETE[] afCompatible;
 
 	// Generate the binding information and save it.
 	pResource->ID = pD3DVBuffer;
 	pResource->VertexSize = vertexSize;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnReleaseVBuffer (ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);
	pResource->ID->Release();
	pResource->Declaration->Release();
	WIRE_DELETE pResource;
}
