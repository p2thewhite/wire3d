#include "WireCullState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateCull, StateGlobal);

//----------------------------------------------------------------------------
StateCull::StateCull()
	:
	Enabled(true),
	CullFace(CM_BACK)
{
}

//----------------------------------------------------------------------------
StateCull::~StateCull()
{
}
