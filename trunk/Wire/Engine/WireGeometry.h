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

	// geometric updates
	virtual void UpdateWorldBound();
	virtual void UpdateModelBound();

	inline VertexBuffer* GetVBuffer();
	inline const VertexBuffer* GetVBuffer() const;
	inline IndexBuffer* GetIBuffer();
	inline const IndexBuffer* GetIBuffer() const;

	void GenerateNormals(Bool ignoreHardEdges = false);

	// member access
	BoundingVolumePtr ModelBound;
	StatePtr States[State::MAX_STATE_TYPE];
	TArray<LightPtr> Lights;

protected:
	// render state updates
	virtual void UpdateState(TArray<State*>* pGStack, TArray<Light*>*
		pLStack);

	// culling
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

private:
	VertexBufferPtr mspVBuffer;
	IndexBufferPtr mspIBuffer;
};

typedef Pointer<Geometry> GeometryPtr;
#include "WireGeometry.inl"

}

#endif
