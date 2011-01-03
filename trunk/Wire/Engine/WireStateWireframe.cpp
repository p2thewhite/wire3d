#include "WireStateWireframe.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateWireframe, State);

//----------------------------------------------------------------------------
StateWireframe::StateWireframe()
	:
	Enabled(false)
{
}

//----------------------------------------------------------------------------
StateWireframe::~StateWireframe()
{
}
