#include "WireStateFog.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, StateFog, State);

//----------------------------------------------------------------------------
StateFog::StateFog()
	:
	Enabled(false),
	Color(ColorRGB::BLACK),
	DensityFunc(DF_LINEAR),
	Start(0.0F),
	End(1.0F)
{
}

//----------------------------------------------------------------------------
StateFog::~StateFog()
{
}
