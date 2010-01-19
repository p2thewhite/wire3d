#include "WireGlobalState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(GlobalState, Object);

GlobalStatePtr GlobalState::Default[GlobalState::MAX_STATE_TYPE];

//----------------------------------------------------------------------------
GlobalState::GlobalState()
{
}

//----------------------------------------------------------------------------
GlobalState::~GlobalState()
{
}
