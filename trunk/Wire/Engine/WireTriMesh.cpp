#include "WireTriMesh.h"

using namespace Wire;

//----------------------------------------------------------------------------
TriMesh::TriMesh(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer)
	:
	Geometry(pVBuffer, pIBuffer)
{
//	Type = GT_TRIMESH;
}

//----------------------------------------------------------------------------
TriMesh::~TriMesh()
{
}
