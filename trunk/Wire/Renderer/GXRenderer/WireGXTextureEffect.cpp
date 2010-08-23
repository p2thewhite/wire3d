#include "WireRenderer.h"

#include "WireGXRendererData.h"
#include "WireTextureEffect.h"

using namespace Wire;

UChar PdrRendererData::sTexBlend[TextureEffect::BM_QUANTITY] =
{
	GX_REPLACE,			// TextureEffect::BM_REPLACE
	GX_MODULATE,		// TextureEffect::BM_MODULATE
	GX_PASSCLR			// TextureEffect::BM_PASS
};

//----------------------------------------------------------------------------
void Renderer::ApplyEffect(TextureEffect* pEffect)
{
	WIRE_ASSERT(pEffect->Textures.GetQuantity() ==
		pEffect->BlendOps.GetQuantity());

	UInt textureCount = pEffect->Textures.GetQuantity();

	for (UInt unit = 0; unit < textureCount; unit++)
	{
		GXSetTevOp(GX_TEVSTAGE0 + unit, PdrRendererData::sTexBlend[pEffect->
			BlendOps[unit]]);
	}

	GXSetNumTexGens(textureCount);
	GXSetNumTevStages(textureCount);
}
