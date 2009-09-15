#include "WireSpatial.h"

#include "WireCuller.h"

using namespace Wire;

//----------------------------------------------------------------------------
Spatial::Spatial()
	:
	WorldIsCurrent(false),
	WorldBound(BoundingVolume::Create()),
	WorldBoundIsCurrent(false),
	mpParent(NULL)
{
}

//----------------------------------------------------------------------------
Spatial::~Spatial()
{
}

//----------------------------------------------------------------------------
void Spatial::UpdateGS(Double appTime, Bool isInitiator)
{
	UpdateWorldData(appTime);
	UpdateWorldBound();
	if (isInitiator)
	{
		PropagateBoundToRoot();
	}
}

//----------------------------------------------------------------------------
void Spatial::UpdateBS()
{
	UpdateWorldBound();
	PropagateBoundToRoot();
}

//----------------------------------------------------------------------------
void Spatial::UpdateWorldData(Double appTime)
{
	// TODO: update controllers, states, light

	// update world transforms
	if (!WorldIsCurrent)
	{
		if (mpParent)
		{
			World.Product(mpParent->World, Local);
		}
		else
		{
			World = Local;
		}
	}
}

//----------------------------------------------------------------------------
void Spatial::PropagateBoundToRoot()
{
	if (mpParent)
	{
		mpParent->UpdateWorldBound();
		mpParent->PropagateBoundToRoot();
	}
}

//----------------------------------------------------------------------------
void Spatial::OnGetVisibleSet(Culler& rCuller, Bool noCull)
{
	if (Culling == CULL_ALWAYS)
	{
		return;
	}

	if (Culling == CULL_NEVER)
	{
		noCull = true;
	}

	UInt savePlaneState = rCuller.GetPlaneState();
	if (noCull || rCuller.IsVisible(WorldBound))
	{
		GetVisibleSet(rCuller,noCull);
	}

	rCuller.SetPlaneState(savePlaneState);
}
