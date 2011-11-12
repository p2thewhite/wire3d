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
	mpIndices = const_cast<UInt*>(pIndexBuffer->GetData());

	Renderer::Statistics* pStatistics = const_cast<Renderer::Statistics*>(
		Renderer::GetStatistics());
	WIRE_ASSERT(pStatistics);
	pStatistics->IBOCount++;
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

	Renderer::Statistics* pStatistics = const_cast<Renderer::Statistics*>(
		Renderer::GetStatistics());
	WIRE_ASSERT(pStatistics);
	pStatistics->IBOCount--;
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
