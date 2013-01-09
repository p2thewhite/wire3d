// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
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
class Texture2D;

class Text : public Geometry
{
	WIRE_DECLARE_RTTI

public:
	Text(UInt fontHeight, Texture2D* pFontTexture, TArray<Vector2F>& rUvs,
		TArray<Vector4F>& rCharSizes, UInt maxLength);
	virtual ~Text();

	void Clear();
	void Clear(const Color32& rColor);

	// Wire::Text coordinates use the OpenGL convention of (0,0) being the
	// bottom left corner with ascending Y going towards the top. Same as
	// orthogonal projection of Wire::Camera
	void SetPen(Float x, Float y);
	void SetColor(const Color32& rColor);
	Bool Set(const Char* pText, Float x = 0.0F, Float y = 0.0F);
	Bool Set(const Char* pText, const Color32& rColor, Float x = 0.0F,
		Float y = 0.0F);
	Bool Append(const Char* pText);
	Bool Append(const Char* pText, const Color32& rColor);
	Bool Append(const Char* pText, Float x, Float y);
	Bool Append(const Char* pText, const Color32& rColor, Float x, Float y);

	Float GetFontHeight();

	void SetLineWidth(Float lineWidth);
	void SetLineWidth(UInt lineWidth);

	// override the default whitespace-width supplied in the constructor
	void SetWhitespaceWidth(Float whitespaceWidth);

	// when text was changed after rendering it, update the hardware buffers
	// to pick up the changes
	void Update(Renderer* pRenderer);

private:
	TArray<Vector2F> mUvs;			// 4 per font character
	TArray<Vector4F> mCharSizes;	// 1 (width, height, stride x, offset y)
	Color32 mColor;
	Float mFontHeight;
	Float mLineWidth;
	Float mPenX;
	Float mPenY;
	Float mWhitespaceWidth;
	Bool mIsPdrBufferOutOfDate;
};

typedef Pointer<Text> TextPtr;

}

#endif
