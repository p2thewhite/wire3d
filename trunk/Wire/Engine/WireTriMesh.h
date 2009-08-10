#pragma once
#ifndef WIRETRIMESH_H
#define WIRETRIMESH_H

#include "WireGeometry.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ TriMesh : public Geometry
{
public:
	TriMesh(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);
	virtual ~TriMesh();
};

typedef Pointer<TriMesh> TriMeshPtr;

}

#endif
