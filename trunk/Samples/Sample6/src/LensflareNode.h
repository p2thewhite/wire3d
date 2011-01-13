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
	enum FlareType
	{
		FT_FLARE_1,
		FT_FLARE_2,
		FT_FLARE_3,
		FT_FLARE_4,
		FT_FLARE_5
	};

	struct FlareDef
	{
		FlareType Type;
		Float SizeFactor;
		Float PositionFactor;
		ColorRGB Color;
	};

	void CreateFlares();
	Geometry* CreateFlare(const FlareDef& rDef);
	void CreateTextures();
	Float SmoothStep(Float a, Float b, Float x);
	void DrawParticle(Float* const pDst, Float fx, Float fy, UInt width);

	Texture2DPtr mspLensTex0;
	Texture2DPtr mspLensTex1;

	static const FlareDef mDefaultDefs[];
};

typedef Pointer<LensflareNode> LensflareNodePtr;

#endif
