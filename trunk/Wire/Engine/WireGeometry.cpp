#include "WireGeometry.h"

using namespace Wire;

//----------------------------------------------------------------------------
Geometry::Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer)
 	:
 	VBuffer(pVBuffer),
	IBuffer(pIBuffer)
{
}

//----------------------------------------------------------------------------
Geometry::~Geometry()
{
}
