#include "WireMaterialState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(MaterialState, GlobalState);
WIRE_IMPLEMENT_INITIALIZE(MaterialState);
WIRE_IMPLEMENT_TERMINATE(MaterialState);

//----------------------------------------------------------------------------
void MaterialState::Initialize()
{
	Default[MATERIAL] = WIRE_NEW MaterialState;
}

//----------------------------------------------------------------------------
void MaterialState::Terminate()
{
	Default[MATERIAL] = NULL;
}

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
