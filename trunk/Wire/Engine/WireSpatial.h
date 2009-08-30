#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

#include "WireBoundingVolume.h"
#include "WireObject.h"
#include "WireTransformation.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Spatial : public Object
{
public:
	// abstract base class
	virtual ~Spatial();

	Transformation Local;
	Transformation World;

	// World bound access.  In some situations you might want to set the
	// world bound directly and bypass the Spatial::UpdateGS() mechanism.  If
	// WorldBound is set directly, the WorldBoundIsCurrent flag should be set
	// to true.
	BoundingVolumePtr WorldBound;

protected:
	Spatial();

	// geometric updates
	virtual void UpdateWorldBound() = 0;
};

typedef Pointer<Spatial> SpatialPtr;

}

#endif
