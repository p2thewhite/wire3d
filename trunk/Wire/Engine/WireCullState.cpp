#include "WireCullState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(CullState, GlobalState);
WIRE_IMPLEMENT_INITIALIZE(CullState);
WIRE_IMPLEMENT_TERMINATE(CullState);

//----------------------------------------------------------------------------
void CullState::Initialize()
{
	Default[CULL] = WIRE_NEW CullState;
}

//----------------------------------------------------------------------------
void CullState::Terminate()
{
	Default[CULL] = NULL;
}

//----------------------------------------------------------------------------
CullState::CullState()
	:
	Enabled(true),
	CullFace(CM_BACK)
{
}

//----------------------------------------------------------------------------
CullState::~CullState()
{
}
