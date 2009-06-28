#include "WireTriMesh.h"

using namespace Wire;

//----------------------------------------------------------------------------
TriMesh::TriMesh(VertexBuffer* pVBuffer)
	:
	Geometry(pVBuffer)
{
	Type = GT_TRIMESH;
}

//----------------------------------------------------------------------------
TriMesh::~TriMesh()
{
}
