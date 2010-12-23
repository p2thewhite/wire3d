#include "WireFogState.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, FogState, GlobalState);

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
