#pragma once
#ifndef WIRETEXTUREEFFECT_H
#define WIRETEXTUREEFFECT_H

#include "WireEffect.h"
#include "WireTArray.h"
#include "WireTexture2D.h"

namespace Wire
{

class TextureEffect : public Effect
{
	WIRE_DECLARE_RTTI;

	enum BlendMode
	{
		BM_REPLACE,
		BM_MODULATE,
		BM_QUANTITY
	};

public:
	TextureEffect();
	virtual ~TextureEffect();

	TArray<Texture2DPtr> Textures;
	TArray<BlendMode> BlendOps;
};

typedef Pointer<TextureEffect> TextureEffectPtr;

}

#endif
