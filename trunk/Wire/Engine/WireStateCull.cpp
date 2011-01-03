#include "WireStateCull.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateCull, State);

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
