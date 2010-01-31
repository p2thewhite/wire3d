#pragma once
#ifndef WIRETEXTUREEFFECT_H
#define WIRETEXTUREEFFECT_H

#include "WireEffect.h"

namespace Wire
{

class TextureEffect : public Effect
{
	WIRE_DECLARE_RTTI;

public:
	TextureEffect();
	virtual ~TextureEffect();

};

typedef Pointer<TextureEffect> TextureEffectPtr;

}

#endif