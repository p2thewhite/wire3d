// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
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
	size_t size = pIndexBuffer->GetQuantity() * sizeof(UShort);
	System::Memcpy(mpBuffer, size, pIndexBuffer->GetData(), size);
}
