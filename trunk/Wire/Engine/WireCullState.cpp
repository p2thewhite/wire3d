#include "WireCullState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(CullState, GlobalState);

//----------------------------------------------------------------------------
CullState::CullState()
	:
	Enabled(true),
	CullFace(CM_BACK)
{
}

//----------------------------------------------------------------------------
CullState::~CullState()
{
}
