#pragma once
#ifndef WIRETEXTURE_H
#define WIRETEXTURE_H

#include "WireBindable.h"
#include "WireImage2D.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Texture2D : public Object, public Bindable
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

	Texture2D(Image2D* pImage);
	virtual ~Texture2D();

	Image2D* GetImage();
	const Image2D* GetImage() const;

	// Access to filter modes. The default is LINEAR.
	virtual void SetFilterType(FilterType filterType);
	FilterType GetFilterType() const;

	// Access to wrap modes. The defaults are WT_CLAMP.
	void SetWrapType(UInt i, WrapType wrapType);
	WrapType GetWrapType(UInt i) const;

	// Access to the anisotropy value for image filtering.
	void SetAnisotropyValue(Float anisotropy);
	Float GetAnisotropyValue() const;

private:
	Image2DPtr mspImage;
	FilterType mFilterType;	// default = FT_LINEAR
	WrapType mWarpType[2];	// default = WT_CLAMP
	Float mAnisotropy;		// default = 1.0F
};

typedef Pointer<Texture2D> TexturePtr;
#include "WireTexture2D.inl"

}

#endif
