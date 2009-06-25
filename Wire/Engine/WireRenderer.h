#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "WireGeometry.h"

namespace Wire
{

class Camera;

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
	virtual Bool BeginScene(Camera* pCamera);
	virtual void EndScene();

	// Access to the color value used for clearing the back buffer.
	virtual void SetClearColor(const ColorRGBA& rClearColor);
	const ColorRGBA& GetClearColor() const;

	// Window parameters.
	virtual Int GetWidth() const;
	virtual Int GetHeight() const;

protected:
	Renderer();	

	// Current Geometry object for drawing.
	Geometry* mpGeometry;

	// The camera for establishing the view frustum.
	Camera* mpCamera;

	ColorRGBA mClearColor;
};

#include "WireRenderer.inl"

}

#endif /* WIRERENDERER_H */
