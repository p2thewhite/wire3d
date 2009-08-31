#include "WireSpatial.h"

using namespace Wire;

//----------------------------------------------------------------------------
Spatial::Spatial()
	:
	WorldBound(BoundingVolume::Create())
{
}

//----------------------------------------------------------------------------
Spatial::~Spatial()
{
}
