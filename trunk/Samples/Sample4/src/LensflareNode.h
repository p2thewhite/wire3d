#pragma once
#ifndef LENSFLARENODE_H
#define LENSFLARENODE_H

#include "WireEngine.h"

using namespace Wire;


class LensflareNode : public Node
{
public:
	LensflareNode();

protected:
	// we handle culling, so we override GetVisibleSet() from the parent class
	virtual void GetVisibleSet(Culler& rCuller, Bool noCull);

private:
	void CreateFlares();
	Geometry* CreateQuad(Float scale, Float uvFactor, Float uOffset,
		Float vOffset, Texture2D* pTexture);
	void CreateTextures();
	Float SmoothStep(Float a, Float b, Float x);
	void DrawParticle(Float* const pDst, Float fx, Float fy, UInt width);

	Texture2DPtr mspLensTex0;
	Texture2DPtr mspLensTex1;
};

typedef Pointer<LensflareNode> LensflareNodePtr;

#endif
