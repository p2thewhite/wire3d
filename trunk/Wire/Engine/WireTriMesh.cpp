#include "WireTriMesh.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(TriMesh, Geometry);

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
