#pragma once
#ifndef WIREDX9RENDERER_H
#define WIREDX9RENDERER_H

#include "../../Engine/WireEngine.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ Dx9Renderer : public Renderer
{

typedef Renderer Parent;

public:
	Dx9Renderer();
	virtual ~Dx9Renderer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void DisplayBackBuffer();

private:
};

}

#endif
