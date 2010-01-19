#include "WireWireframeState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(WireframeState, GlobalState);
WIRE_IMPLEMENT_INITIALIZE(WireframeState);
WIRE_IMPLEMENT_TERMINATE(WireframeState);

//----------------------------------------------------------------------------
void WireframeState::Initialize()
{
	Default[WIREFRAME] = WIRE_NEW WireframeState;
}

//----------------------------------------------------------------------------
void WireframeState::Terminate()
{
	Default[WIREFRAME] = NULL;
}

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
