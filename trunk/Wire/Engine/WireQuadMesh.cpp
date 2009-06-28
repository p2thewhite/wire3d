#include "WireQuadMesh.h"

using namespace Wire;

//----------------------------------------------------------------------------
QuadMesh::QuadMesh(VertexBuffer* pVBuffer)
	:
	Geometry(pVBuffer)
{
	Type = GT_QUADMESH;
}

//----------------------------------------------------------------------------
QuadMesh::~QuadMesh()
{
}
