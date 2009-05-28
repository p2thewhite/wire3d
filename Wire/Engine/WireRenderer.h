#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "WireGeometry.h"

namespace Wire
{

class /*WIRE_ENGINE_ITEM*/ Renderer
{
public:
	// Abstract API for renderers.  Each graphics API must implement this
	// layer.
	virtual ~Renderer();

	void Draw(Geometry* pGeometry);

	// The main entry point to drawing in the derived-class renderers.
	virtual void DrawElements() = 0;

	Matrix34f* View; // Will probably become WireCamera later.

protected:
	Renderer();	

	// Current Geometry object for drawing.
	Geometry* mpGeometry;
};

}

#endif /* WIRERENDERER_H */
