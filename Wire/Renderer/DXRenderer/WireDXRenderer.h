#pragma once
#ifndef WIREDXRENDERER_H
#define WIREDXRENDERER_H

#include "../../Engine/WireEngine.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ DXRenderer : public Renderer
{

typedef Renderer Parent;

public:
	DXRenderer();
	virtual ~DXRenderer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void DisplayBackBuffer();

private:
};

}

#endif
