#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

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

protected:
	Spatial();
};

typedef Pointer<Spatial> SpatialPtr;

}

#endif