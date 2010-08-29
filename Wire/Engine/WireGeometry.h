#pragma once
#ifndef WIREGEOMETRY_H
#define WIREGEOMETRY_H

#include "WireBoundingVolume.h"
#include "WireIndexBuffer.h"
#include "WireLight.h"
#include "WireSpatial.h"
#include "WireVertexBuffer.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Geometry : public Spatial
{
	WIRE_DECLARE_RTTI;

public:
	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);
	virtual ~Geometry();

	virtual void UpdateWorldBound();

	// member access
	VertexBufferPtr VBuffer;
	IndexBufferPtr IBuffer;
	BoundingVolumePtr ModelBound;

	GlobalStatePtr States[GlobalState::MAX_STATE_TYPE];
	TArray<LightPtr> Lights;

protected:
	// geometric updates
	virtual void UpdateModelBound();

	// render state updates
	virtual void UpdateState(TArray<GlobalState*>* pGStack,
		TArray<Light*>* pLStack);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);
};

typedef Pointer<Geometry> GeometryPtr;

}

#endif
