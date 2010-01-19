#include "WireZBufferState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(ZBufferState, GlobalState);
WIRE_IMPLEMENT_INITIALIZE(ZBufferState);
WIRE_IMPLEMENT_TERMINATE(ZBufferState);

//----------------------------------------------------------------------------
void ZBufferState::Initialize()
{
	Default[ZBUFFER] = WIRE_NEW ZBufferState;
}

//----------------------------------------------------------------------------
void ZBufferState::Terminate()
{
	Default[ZBUFFER] = NULL;
}

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
