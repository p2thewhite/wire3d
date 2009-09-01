#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "../Foundation/WireColorRGBA.h"
#include "../Foundation/WireMatrix34.h"

namespace Wire
{

class Camera;
class Geometry;

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

	void OnFrameChange();

protected:
	Renderer();	

	// Current Geometry object for drawing.
	Geometry* mpGeometry;

	// The camera for establishing the view frustum.
	Camera* mpCamera;

	ColorRGBA mClearColor;

	// Transforms world space to camera space.
	Matrix34F mViewMatrix;
};

#include "WireRenderer.inl"

}

#endif
