#pragma once
#ifndef WIRELIGHTNODE_H
#define WIRELIGHTNODE_H

#include "WireNode.h"

namespace Wire
{

class Light;

class /*WIRE_ENGINE_ITEM*/ LightNode : public Node
{
	WIRE_DECLARE_RTTI;

public:
	LightNode(Light* pLight = NULL);
	virtual ~LightNode();

	void SetLight(Light* pLight);
	Light* GetLight();
	const Light* GetLight() const;

protected:
	// geometric updates
	virtual void UpdateWorldData(double appTime);

private:
	LightPtr mspLight;
};

typedef Pointer<LightNode> LightNodePtr;
#include "WireLightNode.inl"

}

#endif