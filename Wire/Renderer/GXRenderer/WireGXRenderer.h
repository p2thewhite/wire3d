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
	GXRenderer(const ColorRGBA& rClearColor);
	virtual ~GXRenderer();

	virtual Bool BeginScene(Camera* pCamera);
	virtual void EndScene();

	// full window buffer operations
	virtual void ClearBuffers();
	virtual void DisplayBackBuffer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void SetClearColor(const ColorRGBA& rClearColor);

protected:
	// Resource enabling and disabling.
	virtual void EnableIBuffer() {};
	virtual void DisableIBuffer() {};

	// Resource loading and releasing (to/from video memory).
	virtual void OnLoadIBuffer(ResourceIdentifier*& rID,
		IndexBuffer* pBuffer) {};
	virtual void OnReleaseIBuffer(ResourceIdentifier* pID) {};

	// Resource enabling and disabling.
	virtual void OnEnableIBuffer(ResourceIdentifier* pID) {};
	virtual void OnDisableIBuffer(ResourceIdentifier* pID) {};

private:
	GXColor mGXClearColor;

	GXRenderModeObj* mRmode;

	void* mFrameBuffer[2];
	void* mDemoFifoBuffer;
	UInt mFrameBufferIndex;
	Bool mIsFrameBufferDirty;

};

#include "WireGXRenderer.inl"

}

#endif
