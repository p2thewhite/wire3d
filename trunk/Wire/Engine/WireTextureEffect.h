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

	// Texture blending modes.
	// Legend symbols mainly taken from the OpenGL "red book".
	//  C  - result color (RGB)
	//  A  - result alpha
	//  Ct - texture color
	//  At - texture alpha
	//  Cf - fragment (current) color
	//  Af - fragment (current) alpha
	//  C1 - color argument 1
	//  A1 - alpha argument 1
	//  C2 - alpha argument 2
	//  A2 - alpha argument 2

	enum BlendMode
	{
		BM_REPLACE,		// C=Ct, Ct=C1, A=At, At=A1
		BM_MODULATE,	// C=Ct*Cf, Ct=C1, Cf=C2, A=At*Af, At=A1, Af=A2
		BM_PASS,		// C=Cf, A=Af
		BM_BLEND,		// C=Ct+(1-Ct)*Cf, Ct=C1, Cf=C3, A=At*Af, At=A1, Af=A2
		BM_DECAL,		// C=At*Ct+(1-At)*Cf, Ct=C1, Cf=C2, A=Af, Af=A1
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
