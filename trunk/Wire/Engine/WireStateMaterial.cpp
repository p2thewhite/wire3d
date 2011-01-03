#include "WireStateMaterial.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateMaterial, State);

//----------------------------------------------------------------------------
StateMaterial::StateMaterial()
	:
	Ambient(0.2F, 0.2F, 0.2F, 1.0F)
{
}

//----------------------------------------------------------------------------
StateMaterial::~StateMaterial()
{
}
