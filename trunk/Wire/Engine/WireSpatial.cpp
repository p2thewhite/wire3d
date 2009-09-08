#include "WireSpatial.h"

using namespace Wire;

//----------------------------------------------------------------------------
Spatial::Spatial()
	:
	WorldBound(BoundingVolume::Create()),
	mpParent(NULL)
{
}

//----------------------------------------------------------------------------
Spatial::~Spatial()
{
}
