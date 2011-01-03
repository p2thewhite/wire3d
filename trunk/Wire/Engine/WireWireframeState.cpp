#include "WireWireframeState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateWireframe, StateGlobal);

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
