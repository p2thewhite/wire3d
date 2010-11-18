#include "WireMaterialState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(MaterialState, GlobalState);

//----------------------------------------------------------------------------
MaterialState::MaterialState()
	:
	Ambient(0.2F, 0.2F, 0.2F, 1.0F)
{
}

//----------------------------------------------------------------------------
MaterialState::~MaterialState()
{
}
