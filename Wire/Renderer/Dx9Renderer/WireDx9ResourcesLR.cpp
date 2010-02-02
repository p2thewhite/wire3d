#include "WireDx9Renderer.h"

#include "WireDx9Resources.h"
#include "WireIndexBuffer.h"
#include "WireTexture.h"
#include "WireVertexBuffer.h"
#include <d3d9.h>
#include <d3dx9tex.h>

using namespace Wire;

D3DFORMAT Dx9Renderer::ms_aeImageFormat[Image::FM_QUANTITY] =
{
	D3DFMT_A8R8G8B8,      // Image::FM_RGB888
	D3DFMT_A8R8G8B8,      // Image::FM_RGBA8888
};

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
	D3DFORMAT format = D3DFMT_INDEX32;

	if (!mSupports32BitIndices)
	{
		indexBufferSize = quantity * sizeof(UShort);
		format = D3DFMT_INDEX16;
	}

	// Create the index buffer.
	IDirect3DIndexBuffer9* pD3DIBuffer;
	msResult = mpD3DDevice->CreateIndexBuffer(indexBufferSize,
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
 
 	TArray<D3DVERTEXELEMENT9> elements(8);
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
			element.UsageIndex = static_cast<BYTE>(unit);
			elements.Append(element);
 		}
 	}

	WIRE_ASSERT(channels > 0);

 	D3DVERTEXELEMENT9 sentinel = D3DDECL_END();
 	elements.Append(sentinel);
 
 	msResult = mpD3DDevice->CreateVertexDeclaration(&elements[0],
		&pResource->Declaration);
 	WIRE_ASSERT(SUCCEEDED(msResult));
 
 	// Create the vertex buffer.
	UInt vbSize = vertexSize * pVBuffer->GetVertexQuantity();
 	IDirect3DVertexBuffer9* pD3DVBuffer;
 	msResult = mpD3DDevice->CreateVertexBuffer(vbSize, 0, 0, D3DPOOL_MANAGED,
 		&pD3DVBuffer, NULL);
 	WIRE_ASSERT(SUCCEEDED(msResult));
 
 	// Copy the vertex buffer data from system memory to video memory.
 	Float* pVBData;
 	msResult = pD3DVBuffer->Lock(0, vbSize, reinterpret_cast<void**>(
		&pVBData), 0);
 	WIRE_ASSERT(SUCCEEDED(msResult));

	Convert(pVBuffer, pVBData);

	msResult = pD3DVBuffer->Unlock();
 	WIRE_ASSERT(SUCCEEDED(msResult));
 
 	// Generate the binding information and save it.
 	pResource->ID = pD3DVBuffer;
 	pResource->VertexSize = vertexSize;
}

//----------------------------------------------------------------------------
void Dx9Renderer::Convert(const VertexBuffer* pSrc, Float* pDst)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
	{
		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* pPosition = pSrc->GetPosition(i);
			for (UInt k = 0; k < rIAttr.GetPositionChannels(); k++)
			{
				*pDst++ = pPosition[k];
			}
		}

		UInt colorChannelQuantity = rIAttr.GetColorChannelQuantity();
		for (UInt unit = 0; unit < colorChannelQuantity; unit++)
		{
			if (rIAttr.GetColorChannels(unit) > 0)
			{
				const Float* pColor = pSrc->GetColor(i, unit);
				D3DCOLOR color = 0xffffffff;
				for (UInt k = 0; k < rIAttr.GetColorChannels(unit); k++)
				{
					color = color << 8;
					color |= static_cast<UChar>(pColor[k] * 255.0F);
				}

				if (rIAttr.GetColorChannels(unit) == 4)
				{
					UChar alpha = static_cast<UChar>(color);
					color = color >> 8;
					color |= alpha << 24;
				}

				DWORD* pColorDst = reinterpret_cast<DWORD*>(pDst);
				*pColorDst++ = color;
				pDst = reinterpret_cast<Float*>(pColorDst);
			}
		}
	}
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnReleaseVBuffer(ResourceIdentifier* pID)
{
	VBufferID* pResource = static_cast<VBufferID*>(pID);
	pResource->ID->Release();
	pResource->Declaration->Release();
	WIRE_DELETE pResource;
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnLoadTexture(ResourceIdentifier*& rID, Texture* pTexture)
{
	// The texture is encountered the first time. Set up the texture unit
	// in hardware that will manage this texture.
	TextureID* pResource = WIRE_NEW TextureID;
	pResource->TextureObject = pTexture;
	rID = pResource;

	// Copy the image data from system memory to video memory.
	const Image* pImage = pTexture->GetImage();
	WIRE_ASSERT(pImage);

	// Windows stores BGR (lowest byte to highest byte), but Wild Magic
	// stores RGB.  The byte ordering must be reversed.

	int iQuantity, iByteQuantity = 0;
	UChar* aucSrc = pImage->GetData();
	UChar* aucRSrc = 0;
	Bool bOwnRSrc = true;
	int i, iSrcBase = 0, iRSrcBase = 0;
	Image::FormatMode eFormat = pImage->GetFormat();
	D3DFORMAT eD3DFormat = ms_aeImageFormat[eFormat];

	if (aucSrc)
	{
		switch (eFormat)
		{
		case Image::FM_RGB888:
			// Swap R and B and pad to an RGBA image.
			iQuantity = pImage->GetQuantity();
			iByteQuantity = 4*iQuantity;
			aucRSrc = WIRE_NEW unsigned char[iByteQuantity];
			for (i = 0; i < iQuantity; i++, iSrcBase += 3, iRSrcBase += 4)
			{
				aucRSrc[iRSrcBase    ] = aucSrc[iSrcBase + 2];
				aucRSrc[iRSrcBase + 1] = aucSrc[iSrcBase + 1];
				aucRSrc[iRSrcBase + 2] = aucSrc[iSrcBase    ];
				aucRSrc[iRSrcBase + 3] = 255;
			}
			break;

		case Image::FM_RGBA8888:
			iQuantity = pImage->GetQuantity();
			iByteQuantity = 4*iQuantity;
			aucRSrc = WIRE_NEW unsigned char[iByteQuantity];
			for (i = 0; i < iQuantity; i++, iSrcBase += 4, iRSrcBase += 4)
			{
				aucRSrc[iRSrcBase    ] = aucSrc[iSrcBase + 2];
				aucRSrc[iRSrcBase + 1] = aucSrc[iSrcBase + 1];
				aucRSrc[iRSrcBase + 2] = aucSrc[iSrcBase    ];
				aucRSrc[iRSrcBase + 3] = aucSrc[iSrcBase + 3];
			}
			break;

		default:
			// There is no need to preprocess depth or intensity images. The
			// floating-point formats and the 16/32-bit integer formats are
			// already RGB/RGBA.
			aucRSrc = aucSrc;
			bOwnRSrc = false;
			break;
		}
	}

	DWORD dwUsage = D3DUSAGE_AUTOGENMIPMAP;
	D3DPOOL kPool = D3DPOOL_MANAGED;
	D3DLOCKED_RECT kLockRect;
	LPDIRECT3DTEXTURE9 pkDXTexture;

	msResult = D3DXCreateTexture(mpD3DDevice, pImage->GetBound(0),
		pImage->GetBound(1), 0, dwUsage, eD3DFormat, kPool, &pkDXTexture);
	WIRE_ASSERT(SUCCEEDED(msResult));

	if (aucRSrc)
	{
		msResult = pkDXTexture->LockRect(0, &kLockRect, 0, 0);
		WIRE_ASSERT(SUCCEEDED(msResult));
		memcpy(kLockRect.pBits, aucRSrc, iByteQuantity);
		msResult = pkDXTexture->UnlockRect(0);
		WIRE_ASSERT(SUCCEEDED(msResult));
	}

	pResource->ID = pkDXTexture;

	if (bOwnRSrc)
	{
		WIRE_DELETE[] aucRSrc;
	}
}

//----------------------------------------------------------------------------
void Dx9Renderer::OnReleaseTexture(ResourceIdentifier* pID)
{
	TextureID* pResource = static_cast<TextureID*>(pID);
	msResult = pResource->ID->Release();
	WIRE_ASSERT(SUCCEEDED(msResult));
	WIRE_DELETE pResource;
}
