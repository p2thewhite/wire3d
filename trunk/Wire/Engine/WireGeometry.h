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
	enum GeometryType
	{
		GT_TRIMESH,
		GT_MAX_QUANTITY
	};

	// abstract base class
	virtual ~Geometry();

	// member access
	VertexBufferPtr VBuffer;
	IndexBufferPtr IBuffer;
	BoundingVolumePtr ModelBound;

// TODO: remove Type/TriMesh class
//	GeometryType Type;
	GlobalStatePtr States[GlobalState::MAX_STATE_TYPE];
	TArray<LightPtr> Lights;

protected:
	Geometry(VertexBuffer* pVBuffer, IndexBuffer* pIBuffer);

	// geometric updates
	virtual void UpdateModelBound();
	virtual void UpdateWorldBound();

	// render state updates
	virtual void UpdateState(TArray<GlobalState*>* pGStack,
		TArray<Light*>* pLStack);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);
};

typedef Pointer<Geometry> GeometryPtr;

}

#endif
