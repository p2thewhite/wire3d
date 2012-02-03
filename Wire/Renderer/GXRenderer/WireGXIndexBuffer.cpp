// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXIndexBuffer.h"

#include "WireIndexBuffer.h"
#include "WireGXDisplayList.h"
#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer* pRenderer, const IndexBuffer*
	pIndexBuffer)
{
	CreateBuffer(NULL, pIndexBuffer->GetQuantity() * sizeof(UShort),
		pIndexBuffer->GetUsage());

	Update(pIndexBuffer);
}

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer*, UInt size, Buffer::UsageType usage)
{
	CreateBuffer(NULL, size, usage);
}

//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer()
{
	TArray<TMap<UInt, PdrDisplayList*>::MapElement>* pArray = mDisplayLists.
		GetArray();

	for (UInt i = 0; i < pArray->GetQuantity(); i++)
	{
		WIRE_DELETE (*pArray)[i].Value;
	}

	WIRE_DELETE[] mpBuffer;
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::CreateBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
{
	mBufferSize = size;
	mpBuffer = reinterpret_cast<UShort*>(memalign(32, mBufferSize));
	WIRE_ASSERT(mpBuffer);
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable(Renderer* pRenderer)
{
	PdrRendererData* pData = pRenderer->GetRendererData();
	pData->PdrIBuffer = this;
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable(Renderer* pRenderer)
{
	PdrRendererData* pData = pRenderer->GetRendererData();
	pData->PdrIBuffer = NULL;
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Update(const IndexBuffer* pIndexBuffer)
{
	Copy(pIndexBuffer, mpBuffer, 0);
}

//----------------------------------------------------------------------------
void PdrIndexBuffer::Copy(const IndexBuffer* pIndexBuffer, void* pBuffer,
	UInt offset)
{
	const UInt* pIndices = pIndexBuffer->GetData();
	const UInt quantity = pIndexBuffer->GetQuantity();

	if (offset == 0)
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

		return;
	}

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
