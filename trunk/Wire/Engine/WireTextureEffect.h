#pragma once
#ifndef WIRETEXTUREEFFECT_H
#define WIRETEXTUREEFFECT_H

#include "WireEffect.h"
#include "WireTexture.h"

namespace Wire
{

class TextureEffect : public Effect
{
	WIRE_DECLARE_RTTI;

public:
	TextureEffect();
	virtual ~TextureEffect();

	TArray<TexturePtr> Textures;
};

typedef Pointer<TextureEffect> TextureEffectPtr;

}

#endif
