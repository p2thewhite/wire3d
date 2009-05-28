#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireSpatial.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Geometry : public Spatial
{
public:
	enum GeometryType
	{
		GT_TRIMESH,
		GT_QUADMESH,
		GT_MAX_QUANTITY
	};

	Geometry(VertexBuffer* pVBuffer);
	virtual ~Geometry();

	// member access
	GeometryType Type;
	VertexBufferPtr VBuffer;
};

typedef Pointer<Geometry> GeometryPtr;

}

#endif /* WIREGEOMETRY_H */
