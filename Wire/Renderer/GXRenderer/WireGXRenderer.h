#pragma once
#ifndef WIREGXRENDERER_H
#define WIREGXRENDERER_H

#include "WireCamera.h"
#include "WireMatrix34.h"
#include "WireRenderer.h"

namespace Wire
{

class VBufferID;

class /*WIRE_RENDERER_ITEM*/ GXRenderer : public Renderer
{

typedef Renderer Parent;

public:
	GXRenderer(const ColorRGBA& rClearColor);
	virtual ~GXRenderer();

	virtual Bool BeginScene(Camera* pCamera);
	virtual void EndScene();

	// Apply camera changes to platform specific renderer.
	virtual void OnFrameChange();

	// full window buffer operations
	virtual void ClearBuffers();
	virtual void DisplayBackBuffer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void SetClearColor(const ColorRGBA& rClearColor);

protected:
	// Resource enabling and disabling.
	virtual void EnableIBuffer() {}

	// Resource loading and releasing (to/from video memory).
	virtual void OnLoadIBuffer(ResourceIdentifier*& rID,
		IndexBuffer* pBuffer) {};
	virtual void OnReleaseIBuffer(ResourceIdentifier* pID) {}

	virtual void OnLoadVBuffer(ResourceIdentifier*& rID,
		VertexBuffer* pVBuffer);
	virtual void OnReleaseVBuffer(ResourceIdentifier* pID);

	virtual void OnLoadTexture(ResourceIdentifier*& rID,
		Texture* pTexture) {} 	// TODO
	virtual void OnReleaseTexture(ResourceIdentifier* pkID) {} 	// TODO

	// Resource enabling and disabling.
	virtual void OnEnableIBuffer(ResourceIdentifier* pID) {}

	virtual void OnEnableVBuffer(ResourceIdentifier* pID) {}

	virtual void OnEnableTexture(ResourceIdentifier* pID) {}  // TODO
	virtual void OnDisableTexture(ResourceIdentifier* pID) {}  // TODO

	// global render state management
	virtual void SetAlphaState(AlphaState* pState);
	virtual void SetCullState(CullState* pState);
	virtual void SetFogState(FogState* pState);
	virtual void SetZBufferState(ZBufferState* pState);

private:
	void Convert(const VertexBuffer* pSrc, VBufferID* pResource);

	GXColor mGXClearColor;

	GXRenderModeObj* mRmode;

	void* mFrameBuffer[2];
	void* mDemoFifoBuffer;
	UInt mFrameBufferIndex;
	Bool mIsFrameBufferDirty;

	// Transforms world space to camera space.
	Matrix34F mViewMatrix;

	static UChar msAlphaSrcBlend[];
	static UChar msAlphaDstBlend[];
	static UChar msCullType[];
	static UChar msFogDensity[];
	static UChar msZBufferCompare[];
};

}

#endif
