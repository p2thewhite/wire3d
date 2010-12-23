#include "WireWireframeState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, WireframeState, GlobalState);

//----------------------------------------------------------------------------
WireframeState::WireframeState()
	:
	Enabled(false)
{
}

//----------------------------------------------------------------------------
WireframeState::~WireframeState()
{
}
