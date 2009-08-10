#pragma once
#ifndef WIREQUADMESH_H
#define WIREQUADMESH_H

#include "WireGeometry.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ QuadMesh : public Geometry
{
public:
	QuadMesh(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);
	virtual ~QuadMesh();
};

typedef Pointer<QuadMesh> QuadMeshPtr;

}

#endif
