#include "WireDx9Renderer.h"

#include "WireDx9Resources.h"

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
	LPDIRECT3DINDEXBUFFER9 pD3DIBuffer;
	msResult = mpDevice->CreateIndexBuffer(indexBufferSize,
		D3DUSAGE_WRITEONLY, format, D3DPOOL_MANAGED, &pD3DIBuffer, NULL);
	WIRE_ASSERT(SUCCEEDED(msResult));

	// Copy the index buffer data from system memory to video memory.
	Char* pLockedIBuffer;
	msResult = pD3DIBuffer->Lock(0, indexBufferSize,
		(void**)(&pLockedIBuffer), 0);
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
				((UShort*)pLockedIBuffer)[i] = static_cast<UShort>(
					pIndices[i]);
				((UShort*)pLockedIBuffer)[i+1] = static_cast<UShort>(
					pIndices[i+1]);
				((UShort*)pLockedIBuffer)[i+2] = static_cast<UShort>(
					pIndices[i+2]);
			}
		}
		else
		{
			for (UInt i = 0; i < quantity; i++)
			{
				((UShort*)pLockedIBuffer)[i] = static_cast<UShort>(
					pIndices[i]);
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
	UInt channels;

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
// 
// 	// Create the vertex buffer.
// 	UInt uiVBSize = (unsigned int)(channels*sizeof(float));
// 	LPDIRECT3DVERTEXBUFFER9 pqVBuffer;
// 	msResult = m_pqDevice->CreateVertexBuffer(uiVBSize,0,0,D3DPOOL_MANAGED,
// 		&pqVBuffer,0);
// 	WIRE_ASSERT(SUCCEEDED(msResult));
// 
// 	// Copy the vertex buffer data from system memory to video memory.
// 	float* afVBData;
// 	msResult = pqVBuffer->Lock(0,uiVBSize,(void**)(&afVBData),0);
// 	WIRE_ASSERT(SUCCEEDED(msResult));
// 	System::Memcpy(afVBData,uiVBSize,afCompatible,uiVBSize);
// 	msResult = pqVBuffer->Unlock();
// 	WIRE_ASSERT(SUCCEEDED(msResult));
// 
// 	WIRE_DELETE[] afCompatible;
// 
// 	// Generate the binding information and save it.
// 	pResource->ID = pqVBuffer;
// 	pResource->VertexSize = vertexSize;
}
