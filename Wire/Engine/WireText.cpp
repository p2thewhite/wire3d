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
	mColor(Color32::WHITE),
	mIsPdrBufferOutOfDate(true)
{
	WIRE_ASSERT(rUvs.GetQuantity() == rCharSizes.GetQuantity()*4);
	WIRE_ASSERT(maxLength < (0x10000/4));

	mFontHeight = static_cast<Float>(fontHeight);
	mUvs = rUvs;
	mCharSizes = rCharSizes;

	VertexAttributes attr;
	attr.SetPositionChannels(3);
	attr.SetTCoordChannels(2);
	attr.SetColorChannels(4);
	VertexBuffer* pVertexBuffer = WIRE_NEW VertexBuffer(attr, maxLength*4,
		Buffer::UT_DYNAMIC_WRITE_ONLY);

	IndexBuffer* pIndexBuffer = WIRE_NEW IndexBuffer(maxLength*3*2,
		Buffer::UT_DYNAMIC_WRITE_ONLY);

	Mesh* pMesh = WIRE_NEW Mesh(pVertexBuffer, pIndexBuffer);
	SetMesh(pMesh);

	Init();
	Clear();
	mWhitespaceWidth = mCharSizes[' '].Z();
	mLineWidth = MathF::MAX_REAL;

	Material* pMaterial = WIRE_NEW Material;
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
	WIRE_ASSERT(GetMesh());
	GetMesh()->SetIndexCount(0);
	mPenX = 0.0F;
	mPenY = 0.0F;
}

//----------------------------------------------------------------------------
void Text::Clear(const Color32& rColor)
{
	mColor = rColor;
	Clear();
}

//----------------------------------------------------------------------------
void Text::SetPen(Float x, Float y)
{
	mPenX = x;
	mPenY = y;
}

//----------------------------------------------------------------------------
void Text::SetColor(const Color32& rColor)
{
	mColor = rColor;
}

//----------------------------------------------------------------------------
Bool Text::Set(const Char* pText, Float x, Float y)
{
	Clear();
	return Append(pText, x, y);
}

//----------------------------------------------------------------------------
Bool Text::Set(const Char* pText, const Color32& rColor, Float x, Float y)
{
	mColor = rColor;
	return Set(pText, x, y);
}

//----------------------------------------------------------------------------
Bool Text::Append(const Char* pText)
{
	return Append(pText, mPenX, mPenY);
}

//----------------------------------------------------------------------------
Bool Text::Append(const Char* pText, const Color32& rColor)
{
	mColor = rColor;
	return Append(pText);
}

//----------------------------------------------------------------------------
Bool Text::Append(const Char* pText, Float x, Float y)
{
	if (!pText)
	{
		return true;
	}

	VertexBuffer* pVertexBuffer = GetMesh()->GetVertexBuffer();
	WIRE_ASSERT(pVertexBuffer && pVertexBuffer->GetAttributes().HasTCoord());
	IndexBuffer* pIndexBuffer = GetMesh()->GetIndexBuffer();
	WIRE_ASSERT(pIndexBuffer);
	const UInt offset = GetMesh()->GetIndexCount()/6;

	const UInt maxLength = pVertexBuffer->GetQuantity() / 4;
	UInt indexCount = 0;
	for (UInt j = 0; pText[j]; j++)
	{
		if ((offset+indexCount) >= maxLength)
		{
			break;
		}

		UInt c = static_cast<UInt>(pText[j]);
		if (c == ' ')
		{
			x+= mWhitespaceWidth;
			continue;
		}

		if (c >= mCharSizes.GetQuantity())		
		{
			return false;
		}

		UInt i = (indexCount + offset) * 4;
		Float cWidth = mCharSizes[c].X();
		Float cHeight = mCharSizes[c].Y();
		Float cStride = mCharSizes[c].Z();
		if ((x+cStride) >= mLineWidth || c == '\n')
		{
			x = 0.0F;
			y -= mFontHeight;

			if (c == '\n')
			{
				continue;
			}
		}

		Float cy = y - mCharSizes[c].W();
		pVertexBuffer->Position3(i) = Vector3F(x, cy+cHeight, 0);
		pVertexBuffer->Color4(i) = mColor;
		pVertexBuffer->Position3(i+1) = Vector3F(x+cWidth, cy+cHeight, 0);
		pVertexBuffer->Color4(i+1) = mColor;
		pVertexBuffer->Position3(i+2) = Vector3F(x+cWidth, cy, 0);
		pVertexBuffer->Color4(i+2) = mColor;
		pVertexBuffer->Position3(i+3) = Vector3F(x, cy, 0);
		pVertexBuffer->Color4(i+3) = mColor;

		UInt c4 = c*4;
		pVertexBuffer->TCoord2(i) = mUvs[c4];
		pVertexBuffer->TCoord2(i+1) = mUvs[c4+1];
		pVertexBuffer->TCoord2(i+2) = mUvs[c4+2];
		pVertexBuffer->TCoord2(i+3) = mUvs[c4+3];

		UInt k = (indexCount + offset) * 6;
		UShort l = static_cast<UShort>(indexCount + offset)*4;
		(*pIndexBuffer)[k] = 0 + l;
		(*pIndexBuffer)[k+1] = 1 + l;
		(*pIndexBuffer)[k+2] = 2 + l;
		(*pIndexBuffer)[k+3] = 0 + l;
		(*pIndexBuffer)[k+4] = 2 + l;
		(*pIndexBuffer)[k+5] = 3 + l;

		x+= cStride;
		indexCount++;

		Culling = CULL_NEVER;
	}

	GetMesh()->SetIndexCount(GetMesh()->GetIndexCount() + indexCount*6);
	mIsPdrBufferOutOfDate = true;

	mPenX = x;
	mPenY = y;

	return true;
}

//----------------------------------------------------------------------------
Bool Text::Append(const Char* pText, const Color32& rColor, Float x, Float y)
{
	mColor = rColor;
	return Append(pText, x, y);
}

//----------------------------------------------------------------------------
void Text::Update(Renderer* pRenderer)
{
	if (!mIsPdrBufferOutOfDate)
	{
		return;
	}

	VertexBuffer* pVertexBuffer = GetMesh()->GetVertexBuffer();
	const UInt indexCount = GetMesh()->GetIndexCount();
	if (pRenderer->GetResource(pVertexBuffer))
	{
		WIRE_ASSERT(indexCount % 6 == 0);
		pRenderer->Update(pVertexBuffer, indexCount/6*4);
	}

	IndexBuffer* pIndexBuffer = GetMesh()->GetIndexBuffer();
	if (pRenderer->GetResource(pIndexBuffer))
	{
		pRenderer->Update(pIndexBuffer, indexCount);
	}

	mIsPdrBufferOutOfDate = false;
}

//----------------------------------------------------------------------------
Float Text::GetFontHeight()
{
	return mFontHeight;
}

//----------------------------------------------------------------------------
void Text::SetLineWidth(Float lineWidth)
{
	mLineWidth = lineWidth;
}

//----------------------------------------------------------------------------
void Text::SetLineWidth(UInt lineWidth)
{
	mLineWidth = static_cast<Float>(lineWidth);
}

//----------------------------------------------------------------------------
void Text::SetWhitespaceWidth(Float whitespaceWidth)
{
	mWhitespaceWidth = whitespaceWidth;
}
