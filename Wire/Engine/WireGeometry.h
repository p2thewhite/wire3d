#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireIndexBuffer.h"
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

	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);
	virtual ~Geometry();

	// member access
	GeometryType Type;
	VertexBufferPtr VBuffer;
	IndexBufferPtr IBuffer;
};

typedef Pointer<Geometry> GeometryPtr;

}

#endif
