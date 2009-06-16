#pragma once
#ifndef WIREGXRENDERER_H
#define WIREGXRENDERER_H

#include "../../Engine/WireEngine.h"

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ GXRenderer : public Renderer
{

typedef Renderer Parent;

public:
	GXRenderer();
	virtual ~GXRenderer();

	virtual Bool BeginScene();
	virtual void EndScene();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void DisplayBackBuffer();

	virtual void SetClearColor(const ColorRGBA& rClearColor);

	virtual Int GetWidth() const;
	virtual Int GetHeight() const;

private:
	GXColor mGXClearColor;

	void* mFrameBuffer[2];
	void* mDemoFifoBuffer;
	UInt mFrameBufferIndex;
	GXRenderModeObj* mRmode;
};

#include "WireGXRenderer.inl"

}

#endif /* WIREGXRENDERER_H */
