#include "WireGeometry.h"
#include "WireCuller.h"

using namespace Wire;

//----------------------------------------------------------------------------
Geometry::Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer)
 	:
 	VBuffer(pVBuffer),
	IBuffer(pIBuffer),
	ModelBound(BoundingVolume::Create())
{
	UpdateModelBound();
}

//----------------------------------------------------------------------------
Geometry::~Geometry()
{
}

//----------------------------------------------------------------------------
void Geometry::UpdateModelBound()
{
	ModelBound->ComputeFromData(VBuffer);
}

//----------------------------------------------------------------------------
void Geometry::UpdateWorldBound()
{
	ModelBound->TransformBy(World, WorldBound);
}

//----------------------------------------------------------------------------
void Geometry::GetVisibleSet(Culler& rCuller, Bool noCull)
{
	rCuller.Insert(this/*, NULL*/);
}
