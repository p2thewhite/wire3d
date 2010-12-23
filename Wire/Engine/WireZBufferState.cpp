#include "WireZBufferState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, ZBufferState, GlobalState);

//----------------------------------------------------------------------------
ZBufferState::ZBufferState()
	:
	Enabled(true),
	Writable(true),
	Compare(CF_LEQUAL)
{
}

//----------------------------------------------------------------------------
ZBufferState::~ZBufferState()
{
}
