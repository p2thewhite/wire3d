#include "WireGeometry.h"
#include "WireCuller.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Geometry, Spatial);

//----------------------------------------------------------------------------
Geometry::Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer)
 	:
 	VBuffer(pVBuffer),
	IBuffer(pIBuffer),
	ModelBound(BoundingVolume::Create())
{
	System::Memset(States, 0, GlobalState::MAX_STATE_TYPE * sizeof(
		GlobalState*));
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
void Geometry::UpdateState(TArray<GlobalState*>* pGStack,
	TArray<Light*>* pLStack)
{
	// update global state
	for (UInt i = 0; i < GlobalState::MAX_STATE_TYPE; i++)
	{
		TArray<GlobalState*>& rState = pGStack[i];
  		States[i] = rState[rState.GetQuantity()-1];
	}

	// update light state
	Lights.RemoveAll();
	Lights.SetQuantity(pLStack->GetQuantity());
	for (UInt i = 0; i < pLStack->GetQuantity(); i++)
	{
		Lights[i] = (*pLStack)[i];
	}
}

//----------------------------------------------------------------------------
void Geometry::GetVisibleSet(Culler& rCuller, Bool)
{
	rCuller.Insert(this, NULL);
}
