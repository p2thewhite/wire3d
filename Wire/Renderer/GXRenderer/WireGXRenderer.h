#pragma once
#ifndef WIREGXRENDERER_H
#define WIREGXRENDERER_H

#include "../../Engine/WireEngine.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ GXRenderer : public Renderer
{
public:
	GXRenderer();
	virtual ~GXRenderer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();
};

}

#endif /* WIREGXRENDERER_H */
