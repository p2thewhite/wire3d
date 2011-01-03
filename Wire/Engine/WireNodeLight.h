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

	LightPtr mspLight;
};

typedef Pointer<NodeLight> NodeLightPtr;
#include "WireNodeLight.inl"

}

#endif
