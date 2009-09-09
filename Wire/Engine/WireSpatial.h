#pragma once
#ifndef WIRESPATIAL_H
#define WIRESPATIAL_H

#include "WireBoundingVolume.h"
#include "WireObject.h"
#include "WireTransformation.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Spatial : public Object
{
public:
	// abstract base class
	virtual ~Spatial();

	// Local and world transforms. In some situations you might need to set
	// the world transform directly and bypass the Spatial::Update()
	// mechanism. If World is set directly, the WorldIsCurrent flag should
	// be set to true. For example, inverse kinematic controllers and skin
	// controllers need this capability.
	Transformation Local;
	Transformation World;
	bool WorldIsCurrent;

	// World bound access. In some situations you might want to set the
	// world bound directly and bypass the Spatial::UpdateGS() mechanism. If
	// WorldBound is set directly, the WorldBoundIsCurrent flag should be set
	// to true.
	BoundingVolumePtr WorldBound;
	bool WorldBoundIsCurrent;

	// Update of geometric state and controllers. The UpdateGS function
	// computes world transformations on the downward pass and world bounding
	// volumes on the upward pass. The UpdateBS function just computes the
	// world bounding volumes on an upward pass. This is useful if model
	// data changes, causing the model and world bounds to change, but no
	// transformations need recomputing.
	void UpdateGS(Double appTime = -MathD::MAX_REAL, Bool isInitiator = true);
	void UpdateBS();

	// Parent access (Node calls this during attach/detach of children)
	void SetParent(Spatial* pkParent);
	Spatial* GetParent();

protected:
	Spatial();

	// geometric updates
	virtual void UpdateWorldData(Double appTime);
	virtual void UpdateWorldBound() = 0;
	void PropagateBoundToRoot();

	// support for hierarchical scene graph
	Spatial* mpParent;
};

typedef Pointer<Spatial> SpatialPtr;
#include "WireSpatial.inl"

}

#endif
