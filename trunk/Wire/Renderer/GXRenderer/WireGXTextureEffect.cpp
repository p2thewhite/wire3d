#include "WireRenderer.h"

#include "WireGXRendererData.h"
#include "WireTextureEffect.h"

using namespace Wire;

UChar PdrRendererData::sTexBlend[TextureEffect::BM_QUANTITY] =
{
	GX_REPLACE,			// TextureEffect::BM_REPLACE
	GX_MODULATE			// TextureEffect::BM_MODULATE
};

//----------------------------------------------------------------------------
void Renderer::ApplyEffect(TextureEffect* pEffect)
{
	WIRE_ASSERT(pEffect->Textures.GetQuantity() ==
		pEffect->BlendOps.GetQuantity());

	for (UInt unit = 0; unit < pEffect->BlendOps.GetQuantity(); unit++)
	{
		GXSetTevOp(GX_TEVSTAGE0, PdrRendererData::sTexBlend[pEffect->BlendOps[
			unit]]);
	}
}
