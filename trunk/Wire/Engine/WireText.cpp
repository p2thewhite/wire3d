// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireText.h"

#include "WireIndexBuffer.h"
#include "WireMesh.h"
#include "WireRenderer.h"
#include "WireStateAlpha.h"
#include "WireVertexAttributes.h"
#include "WireVertexBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Text, Geometry);

//----------------------------------------------------------------------------
Text::Text(UInt fontHeight, Texture2D* pFontTexture, TArray<Vector2F>& rUvs,
	TArray<Vector4F>& rCharSizes, UInt maxLength)
	:
	mIsPdrBufferOutOfDate(true)
{
	WIRE_ASSERT(rUvs.GetQuantity() == rCharSizes.GetQuantity()*4);
	WIRE_ASSERT(maxLength < (0x10000/4));

	mFontHeight = fontHeight;
	mUvs = rUvs;
	mCharSizes = rCharSizes;

	VertexAttributes attr;
	attr.SetPositionChannels(3);
	attr.SetTCoordChannels(2);
	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attr, maxLength*4,
		Buffer::UT_DYNAMIC_WRITE_ONLY);

	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(maxLength*3*2,
		Buffer::UT_DYNAMIC_WRITE_ONLY);

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	SetMesh(pMesh);

	Init();
	Clear();

	Material* pMaterial = WIRE_NEW Material();
	pMaterial->AddTexture(pFontTexture);
	SetMaterial(pMaterial);

	StateAlpha* pAlpha = WIRE_NEW StateAlpha;
	pAlpha->BlendEnabled = true;
	AttachState(pAlpha);
	UpdateRS();
}

//----------------------------------------------------------------------------
Text::~Text()
{
}

//----------------------------------------------------------------------------
void Text::Clear()
{
	Culling = CULL_ALWAYS;
	ActiveIndexCount = 0;
}

//----------------------------------------------------------------------------
Bool Text::Set(const Char* pText, Float x, Float y)
{
	Clear();
	return Add(pText, x, y);
}

//----------------------------------------------------------------------------
Bool Text::Add(const Char* pText, Float x, Float y)
{
	if (!pText)
	{
		return true;
	}

	VertexBuffer* pVertexBuffer = GetMesh()->GetVertexBuffer();
	WIRE_ASSERT(pVertexBuffer && pVertexBuffer->GetAttributes().HasTCoord());
	IndexBuffer* pIndexBuffer = GetMesh()->GetIndexBuffer();
	WIRE_ASSERT(pIndexBuffer);
	const UInt offset = ActiveIndexCount/6;

	// DirectX9 oddity, pixel's center is at (0.5,0.5)
	if (System::GetPlatform() == System::PF_DX9)
	{
		x += 0.5F;
		y += 0.5F;
	}

	const UInt maxLength = pVertexBuffer->GetQuantity() / 4;
	UInt j = 0;
	for (; pText[j]; j++)
	{
		if ((offset+j) >= maxLength)
		{
			break;
		}

		UInt c = static_cast<UInt>(pText[j]);
		if (c == ' ')
		{
			x+= 8;
			continue;
		}

		if (c >= mCharSizes.GetQuantity())		
		{
			return false;
		}

		UInt i = (j + offset) * 4;
		Float cWidth = mCharSizes[c].X();
		Float cHeight = mCharSizes[c].Y();
		Float cStride = mCharSizes[c].Z();
		Float cy = y - mCharSizes[c].W();
		pVertexBuffer->Position3(i) = Vector3F(x, cy+cHeight, 0);
		pVertexBuffer->Position3(i+1) = Vector3F(x+cWidth, cy+cHeight, 0);
		pVertexBuffer->Position3(i+2) = Vector3F(x+cWidth, cy, 0);
		pVertexBuffer->Position3(i+3) = Vector3F(x, cy, 0);

		UInt c4 = c*4;
		pVertexBuffer->TCoord2(i) = mUvs[c4];
		pVertexBuffer->TCoord2(i+1) = mUvs[c4+1];
		pVertexBuffer->TCoord2(i+2) = mUvs[c4+2];
		pVertexBuffer->TCoord2(i+3) = mUvs[c4+3];

		UInt k = (j + offset) * 6;
		UShort l = static_cast<UShort>(j + offset)*4;
		(*pIndexBuffer)[k] = 0 + l;
		(*pIndexBuffer)[k+1] = 1 + l;
		(*pIndexBuffer)[k+2] = 2 + l;
		(*pIndexBuffer)[k+3] = 0 + l;
		(*pIndexBuffer)[k+4] = 2 + l;
		(*pIndexBuffer)[k+5] = 3 + l;

		x+= cStride;
		Culling = CULL_NEVER;
	}

	ActiveIndexCount += j*6;
	mIsPdrBufferOutOfDate = true;

	return true;
}

//----------------------------------------------------------------------------
void Text::Update(Renderer* pRenderer)
{
	if (!mIsPdrBufferOutOfDate)
	{
		return;
	}

	VertexBuffer* pVertexBuffer = GetMesh()->GetVertexBuffer();
	if (pRenderer->GetResource(pVertexBuffer))
	{
		pRenderer->Update(pVertexBuffer);
	}

	IndexBuffer* pIndexBuffer = GetMesh()->GetIndexBuffer();
	if (pRenderer->GetResource(pIndexBuffer))
	{
		pRenderer->Update(pIndexBuffer);
	}

	mIsPdrBufferOutOfDate = false;
}
