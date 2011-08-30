// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRELIGHTNODE_H
#define WIRELIGHTNODE_H

#include "WireNode.h"

namespace Wire
{

class Light;

class NodeLight : public Node
{
	WIRE_DECLARE_RTTI;

public:
	NodeLight(Light* pLight = NULL);
	virtual ~NodeLight();

	void SetLight(Light* pLight);
	inline Light* GetLight();
	inline const Light* GetLight() const;

protected:
	// geometric updates
	virtual void UpdateWorldData(Double appTime);

private:
	void LightToLocalTransform();

	Pointer<Light> mspLight;
};

typedef Pointer<NodeLight> NodeLightPtr;
#include "WireNodeLight.inl"

}

#endif