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

class Geometry : public Spatial
{
	WIRE_DECLARE_RTTI;

public:
	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);
	virtual ~Geometry();

	virtual void UpdateWorldBound();

	void GenerateNormals(Bool ignoreHardEdges = false);

	// member access
	VertexBufferPtr VBuffer;
	IndexBufferPtr IBuffer;
	BoundingVolumePtr ModelBound;

	StateGlobalPtr States[StateGlobal::MAX_STATE_TYPE];
	TArray<LightPtr> Lights;

protected:
	// geometric updates
	virtual void UpdateModelBound();

	// render state updates
	virtual void UpdateState(TArray<StateGlobal*>* pGStack,
		TArray<Light*>* pLStack);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);
};

typedef Pointer<Geometry> GeometryPtr;

}

#endif
