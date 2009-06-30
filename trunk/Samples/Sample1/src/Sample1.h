#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireFoundation.h"
#include "WireEngine.h"
#include "WireGXRenderer.h"
#include "WireGXApplication.h"

class Sample1 : public Wire::GXApplication
{
public:
	Sample1();
	virtual ~Sample1();

	Wire::Geometry* CreateCube();
	Wire::Geometry* CreatePyramid();

	Bool OnInitialize();

	Wire::CameraPtr mspCamera;
	Wire::GeometryPtr mspCube;
	Wire::GeometryPtr mspPyramid;
};

#endif /* SAMPLE1_H */
