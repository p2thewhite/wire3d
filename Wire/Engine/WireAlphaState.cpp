#include "WireAlphaState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(AlphaState, GlobalState);

//----------------------------------------------------------------------------
AlphaState::AlphaState()
	:
	BlendEnabled(false),
	SrcBlend(SBM_SRC_ALPHA),
	DstBlend(DBM_ONE_MINUS_SRC_ALPHA)
{
}

//----------------------------------------------------------------------------
AlphaState::~AlphaState()
{
}