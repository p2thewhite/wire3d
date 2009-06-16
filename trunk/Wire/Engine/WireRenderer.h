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

	virtual void DisplayBackBuffer() = 0;

	// Support for predraw and postdraw semantics.
	virtual Bool BeginScene();
	virtual void EndScene();

	// Access to the color value used for clearing the back buffer.
	virtual void SetClearColor(const ColorRGBA& rClearColor);
	const ColorRGBA& GetClearColor() const;

	// Window parameters.
	virtual Int GetWidth() const;
	virtual Int GetHeight() const;

	Matrix34f* View; // Will probably become WireCamera later.

protected:
	Renderer();	

	// Current Geometry object for drawing.
	Geometry* mpGeometry;

	ColorRGBA mClearColor;
};

#include "WireRenderer.inl"

}

#endif /* WIRERENDERER_H */
