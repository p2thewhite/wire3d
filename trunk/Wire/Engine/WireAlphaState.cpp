#include "WireAlphaState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateAlpha, StateGlobal);

//----------------------------------------------------------------------------
StateAlpha::StateAlpha()
	:
	BlendEnabled(false),
	SrcBlend(SBM_SRC_ALPHA),
	DstBlend(DBM_ONE_MINUS_SRC_ALPHA)
{
}

//----------------------------------------------------------------------------
StateAlpha::~StateAlpha()
{
}
