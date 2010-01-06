#include "WireFogState.h"

using namespace Wire;

WIRE_IMPLEMENT_INITIALIZE(FogState);
WIRE_IMPLEMENT_TERMINATE(FogState);

//----------------------------------------------------------------------------
void FogState::Initialize()
{
	Default[FOG] = WIRE_NEW FogState;
}

//----------------------------------------------------------------------------
void FogState::Terminate()
{
	Default[FOG] = NULL;
}

//----------------------------------------------------------------------------
FogState::FogState()
	:
	Enabled(false),
	Color(ColorRGB::BLACK),
	DensityFunc(DF_LINEAR),
	Start(0.0F),
	End(1.0F)
{
}

//----------------------------------------------------------------------------
FogState::~FogState()
{
}
