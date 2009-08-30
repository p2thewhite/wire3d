#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireBoundingVolume.h"
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

	// abstract base class
	virtual ~Geometry();

	// member access
	VertexBufferPtr VBuffer;
	IndexBufferPtr IBuffer;
	BoundingVolumePtr ModelBound;

	GeometryType Type;

protected:
	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);

	// geometric updates
	virtual void UpdateModelBound();
	virtual void UpdateWorldBound();
};

typedef Pointer<Geometry> GeometryPtr;

}

#endif
