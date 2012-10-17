// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXVertexBuffer.h"

#include "WireEffect.h"
#include "WireGeometry.h"
#include "WireGXRendererData.h"
#include "WireGXVertexAttributes.h"
#include "WireLight.h"
#include "WireMesh.h"
#include "WireRenderer.h"
#include "WireVertexBuffer.h"
#include <malloc.h>

using namespace Wire;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer*, const VertexBuffer* pVertexBuffer)
	:
	mpPdrVertexAttributes(NULL)
{
	CreateDeclaration(NULL, pVertexBuffer->GetAttributes());
	CreateBuffer(NULL, GetVertexSize() * pVertexBuffer->GetQuantity(),
		pVertexBuffer->GetUsage());

	Update(pVertexBuffer);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer*, UInt size, Buffer::UsageType
	usage)
	:
	mpPdrVertexAttributes(NULL)
{
	CreateBuffer(NULL, size, usage);
}

//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	free(mpBuffer);	// allocated using memalign, not using new
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::CreateBuffer(Renderer* pRenderer, UInt size,
	Buffer::UsageType usage)
{
	mBufferSize = size;
	mpBuffer = memalign(32, mBufferSize);
	WIRE_ASSERT(mpBuffer);
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::CreateDeclaration(Renderer*, const VertexAttributes&
	rAttributes)
{
	if (mpPdrVertexAttributes)
	{
		WIRE_DELETE mpPdrVertexAttributes;
	}

	mpPdrVertexAttributes = WIRE_NEW PdrVertexAttributes(NULL, rAttributes);
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer)
{
	Update(pVertexBuffer, pVertexBuffer->GetQuantity());
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Update(const VertexBuffer* pVertexBuffer, UInt count,
	UInt offset)
{
	WIRE_ASSERT(mBufferSize == GetVertexSize() * pVertexBuffer->GetQuantity());
	WIRE_ASSERT(GetVertexSize() == pVertexBuffer->GetAttributes().
		GetChannelQuantity()*sizeof(Float));

	UChar* pBuffer = reinterpret_cast<UChar*>(Lock(Buffer::LM_WRITE_ONLY)) +
		offset * GetVertexSize();
	size_t size = count * GetVertexSize();
	const UChar* pDst = reinterpret_cast<const UChar*>(pVertexBuffer->
		GetData()) + offset * GetVertexSize();
	System::Memcpy(pBuffer, size, pDst, size);
	Unlock();
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer* pRenderer)
{
	SetDeclaration(pRenderer);
	SetBuffer(pRenderer, GetVertexSize());

	pRenderer->GetRendererData()->PdrVBuffer = this;
}

//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer* pRenderer)
{
	pRenderer->GetRendererData()->PdrVBuffer = NULL;
}
