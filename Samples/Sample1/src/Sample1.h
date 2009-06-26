#pragma once
#ifndef SAMPLE1_H
#define SAMPLE1_H

#include "WireFoundation.h"
#include "WireEngine.h"
#include "WireGXRenderer.h"
#include "WireGXApplication.h"

class Sample1 : public Wire::GXApplication
{
private:
	Wire::Geometry* CreateCube();
	Wire::Geometry* CreatePyramid();
};

#endif /* SAMPLE1_H */
