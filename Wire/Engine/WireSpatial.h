#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

#include "WireObject.h"
#include "WireTransformation.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ Spatial : public Object
{
public:
	// abstract base class
	virtual ~Spatial();

	Transformation Local;

protected:
	Spatial();
};

typedef Pointer<Spatial> SpatialPtr;

}

#endif /* WIRESPATIAL_H */
