#include "WireAlphaState.h"

using namespace Wire;

WIRE_IMPLEMENT_INITIALIZE(AlphaState);
WIRE_IMPLEMENT_TERMINATE(AlphaState);

//----------------------------------------------------------------------------
void AlphaState::Initialize()
{
	Default[ALPHA] = WIRE_NEW AlphaState;
}

//----------------------------------------------------------------------------
void AlphaState::Terminate()
{
	Default[ALPHA] = NULL;
}

//----------------------------------------------------------------------------
AlphaState::AlphaState()
	:
	BlendEnabled(false),
	SrcBlend(SBM_SRC_ALPHA),
	DstBlend(DBM_ONE_MINUS_SRC_ALPHA)
{
}

//----------------------------------------------------------------------------
AlphaState::~AlphaState()
{
}
