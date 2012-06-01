// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRETEXT_H
#define WIRETEXT_H

#include "WireGeometry.h"
#include "WireVector2.h"

namespace Wire
{

class Renderer;

class Text : public Geometry
{
	WIRE_DECLARE_RTTI

public:
	Text(UInt fontHeight, Texture2D* pFontTexture, TArray<Vector2F>& rUvs,
		TArray<Vector4F>& rCharSizes, UInt maxLength);
	virtual ~Text();

	void Clear();
	Bool Set(const Char* pText, Float x = 0.0F, Float y = 0.0F);
	Bool Add(const Char* pText, Float x = 0.0F, Float y = 0.0F);
	void Update(Renderer* pRenderer);

private:
	TArray<Vector2F> mUvs;			// 4 per font character
	TArray<Vector4F> mCharSizes;	// 1 (width, height, stride x, offset y)
	UInt mFontHeight;
	Bool mIsPdrBufferOutOfDate;
};

typedef Pointer<Text> TextPtr;

}

#endif
