#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireSpatial.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class /*WIRE_GRAPHICS_ITEM*/ Geometry : public Spatial
{

public:
	Geometry(VertexBuffer* pVBuffer);
	virtual ~Geometry ();

	// member access
	VertexBufferPtr VBuffer;

};

typedef Pointer<Geometry> GeometryPtr;

}

#endif /* WIREGEOMETRY_H */
