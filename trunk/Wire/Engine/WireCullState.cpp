#include "WireCullState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, CullState, GlobalState);

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
