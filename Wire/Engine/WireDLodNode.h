#pragma once
#ifndef WIREDLODNODE_H
#define WIREDLODNODE_H

#include "WireSwitchNode.h"

namespace Wire
{

class Camera;

class /*WIRE_ENGINE_ITEM*/ DLodNode : public SwitchNode
{
	WIRE_DECLARE_RTTI;

public:
	DLodNode();
	virtual ~DLodNode();

	void SetLod(UInt i, Spatial* pLod, Float minDist, Float maxDist);

    // center for level of detail
    inline Vector3F& ModelCenter();
    inline const Vector3F& GetModelCenter() const;
    inline const Vector3F& GetWorldCenter() const;

    // distance intervals for children
    void SetModelDistance(UInt i, Float minDist, Float maxDist);
    inline Float GetModelMinDistance(UInt i) const;
    inline Float GetModelMaxDistance(UInt i) const;
    inline Float GetWorldMinDistance(UInt i) const;
    inline Float GetWorldMaxDistance(UInt i) const;

protected:
    // set the SwitchNode based on distance from world LOD center to camera
    void SelectLevelOfDetail(const Camera* pCamera);

    // culling
    virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

	// point whose distance to camera determines correct child
    Vector3F mModelLodCenter;
    Vector3F mWorldLodCenter;

    // squared distances for each LOD interval
    TArray<Float> mModelMinDist;
    TArray<Float> mModelMaxDist;
    TArray<Float> mWorldMinDist;
    TArray<Float> mWorldMaxDist;
};

typedef Pointer<DLodNode> DLodNodePtr;

#include "WireDLodNode.inl"

}

#endif
