#pragma once
#ifndef SAMPLE6_H
#define SAMPLE6_H

#include "WireApplication.h"

using namespace Wire;

class Sample6 : public WIREAPPLICATION
{
	WIRE_DECLARE_INITIALIZE;

	typedef WIREAPPLICATION Parent;

public:
	virtual Bool OnInitialize();
	virtual void OnIdle();

private:
	struct BlendMode
	{
		BlendMode(TextureEffect::BlendMode mode0,
			TextureEffect::BlendMode mode1)
			:
		Mode0(mode0),
			Mode1(mode1) {}

		TextureEffect::BlendMode Mode0;
		TextureEffect::BlendMode Mode1;
	};

	Geometry* CreateCube(const UInt uvQuantity);
	Texture2D* CreateTexture();
	Texture2D* CreateTexture2();

	CameraPtr mspCamera;
	GeometryPtr mspGeometry;

	Float mAngle;
	Double mLastTime;
};

WIRE_REGISTER_INITIALIZE(Sample6);

#endif
