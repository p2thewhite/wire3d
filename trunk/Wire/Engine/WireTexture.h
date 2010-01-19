#pragma once
#ifndef WIRETEXTURE_H
#define WIRETEXTURE_H

#include "WireBindable.h"
#include "WireImage.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Texture : public Object, public Bindable
{
	WIRE_DECLARE_RTTI;

public:
	enum FilterType
	{
		FT_NEAREST,          // nearest neighbor
		FT_LINEAR,           // linear filtering
		FT_NEAREST_NEAREST,  // nearest within image, nearest across images
		FT_NEAREST_LINEAR,   // nearest within image, linear across images
		FT_LINEAR_NEAREST,   // linear within image, nearest across images
		FT_LINEAR_LINEAR,    // linear within image, linear across images
		FT_QUANTITY
	};

	enum WrapType
	{
		WT_CLAMP,
		WT_REPEAT,
		WT_MIRRORED_REPEAT,
		WT_QUANTITY
	};

	Texture(Image* pImage);
	virtual ~Texture();

private:
	ImagePtr mspImage;
	FilterType mFilterType;	// default = FT_LINEAR
	WrapType mWarpType[2];	// default = WT_CLAMP
};

typedef Pointer<Texture> TexturePtr;

}

#endif
