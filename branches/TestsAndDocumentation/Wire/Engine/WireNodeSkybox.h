// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRENODEKSYBOX_H
#define WIRENODEKSYBOX_H

#include "WireNode.h"

namespace Wire
{

class Image2D;
class Geometry;
class Material;
class Texture2D;

class NodeSkybox : public Node
{
	WIRE_DECLARE_RTTI;

public:
	NodeSkybox(Texture2D* pPosZ, Texture2D* pNegZ, Texture2D* pPosX,
		Texture2D* pNegX, Texture2D* pPosY, Texture2D* pNegY,
		Float scale = 1.0F);
	virtual ~NodeSkybox();

	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

protected:
	virtual void UpdateWorldData(Double appTime);

private:
	void Init(Texture2D* pPosZ, Texture2D* pNegZ, Texture2D* pPosX,
		Texture2D* pNegX, Texture2D* pPosY, Texture2D* pNegY, Float scale);
	void AddQuad(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2,
		const Vector3F& v3, Texture2D* pTexture, const Float scale = 1.0F);

	Double mAppTime;
};

typedef Pointer<NodeSkybox> NodeSkyboxPtr;

}

#endif
