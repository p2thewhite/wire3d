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
#include "WireStateAlpha.h"
#include "WireVertexAttributes.h"
#include "WireVertexBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Text, Geometry);

//----------------------------------------------------------------------------
Text::Text(UInt maxChars, UInt fontHeight, Texture2D* pFontTexture,
	TArray<Vector2F>& rUvs, TArray<Vector2F>& rCharSizes)
{
	WIRE_ASSERT(rUvs.GetQuantity() == rCharSizes.GetQuantity()*4);

	mFontHeight = fontHeight;
	mUvs = rUvs;
	mCharSizes = rCharSizes;

	VertexAttributes attr;
	attr.SetPositionChannels(3);
	attr.SetTCoordChannels(2);
	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attr, maxChars * 4,
		Buffer::UT_DYNAMIC_WRITE_ONLY);

	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(maxChars * 3 * 2,
		Buffer::UT_DYNAMIC_WRITE_ONLY);

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	SetMesh(pMesh);

	Init();
	ActiveIndexCount = 0;
	Culling = CULL_ALWAYS;

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
Bool Text::Set(const Char* pText, Float x, Float y)
{
	Culling = CULL_ALWAYS;
	if (!pText)
	{
		return true;
	}

	ActiveIndexCount = 0;
	return Add(pText, x, y);
}

//----------------------------------------------------------------------------
Bool Text::Add(const Char* pText, Float x, Float y)
{
	VertexBuffer* pVertexBuffer = GetMesh()->GetVertexBuffer();
	WIRE_ASSERT(pVertexBuffer && pVertexBuffer->GetAttributes().HasTCoord());
	IndexBuffer* pIndexBuffer = GetMesh()->GetIndexBuffer();
	WIRE_ASSERT(pIndexBuffer);

	while (*pText)
	{
		UInt c = static_cast<UInt>(*pText++);
		if (c >= mCharSizes.GetQuantity())		
		{
			return false;
		}

		UInt i = ActiveIndexCount*4;
		Float charWidth = mCharSizes[c].X();
		Float charHeight = mCharSizes[c].Y();
		pVertexBuffer->Position3(i) = Vector3F(x, y+charHeight, 0);
		pVertexBuffer->Position3(i+1) = Vector3F(x+charWidth, y+charHeight, 0);
		pVertexBuffer->Position3(i+2) = Vector3F(x+charWidth, y, 0);
		pVertexBuffer->Position3(i+3) = Vector3F(x, y, 0);

		pVertexBuffer->TCoord2(i) = mUvs[i];
		pVertexBuffer->TCoord2(i+1) = mUvs[i+1];
		pVertexBuffer->TCoord2(i+2) = mUvs[i+2];
		pVertexBuffer->TCoord2(i+3) = mUvs[i+3];

		x+=charWidth;
		Culling = CULL_NEVER;
	}

	return true;
}
