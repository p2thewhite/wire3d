#include "WireStateZBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateZBuffer, StateGlobal);

//----------------------------------------------------------------------------
StateZBuffer::StateZBuffer()
	:
	Enabled(true),
	Writable(true),
	Compare(CF_LEQUAL)
{
}

//----------------------------------------------------------------------------
StateZBuffer::~StateZBuffer()
{
}
