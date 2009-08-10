#include "WireQuadMesh.h"

using namespace Wire;

//----------------------------------------------------------------------------
QuadMesh::QuadMesh(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer)
	:
	Geometry(pVBuffer, pIBuffer)
{
	Type = GT_QUADMESH;
}

//----------------------------------------------------------------------------
QuadMesh::~QuadMesh()
{
}
