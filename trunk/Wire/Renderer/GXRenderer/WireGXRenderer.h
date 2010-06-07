#pragma once
#ifndef WIREGXRENDERER_H
#define WIREGXRENDERER_H

#include "WireCamera.h"
#include "WireMatrix34.h"
#include "WireRenderer.h"

namespace Wire
{

class PdrRendererData;
class PdrRendererInput;

class /*WIRE_RENDERER_ITEM*/ GXRenderer : public Renderer
{

public:
	GXRenderer(PdrRendererInput& rInput);
	virtual ~GXRenderer();

	Bool BeginScene(Camera* pCamera);
	void EndScene();

	// Apply camera changes to platform specific renderer.
	virtual void OnFrameChange();
	virtual void OnViewportChange();

	// full window buffer operations
	virtual void ClearBuffers();
	virtual void DisplayBackBuffer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void SetClearColor(const ColorRGBA& rClearColor);

	// use by System::Assert on Wii
	PdrRendererData* GetRendererData() { return mpData; }

protected:
	// Resource loading and releasing (to/from video memory).
	virtual void OnLoadIBuffer(ResourceIdentifier*& rID,
		IndexBuffer* pBuffer);
	virtual void OnReleaseIBuffer(ResourceIdentifier* pID);

	virtual void OnLoadVBuffer(ResourceIdentifier*& rID,
		VertexBuffer* pVBuffer);
	virtual void OnReleaseVBuffer(ResourceIdentifier* pID);

	virtual void OnLoadTexture(ResourceIdentifier*& rID, Texture* pTexture);
	virtual void OnReleaseTexture(ResourceIdentifier* pID);

	// Resource enabling and disabling.
	virtual void OnEnableIBuffer(ResourceIdentifier* pID);
	virtual void OnDisableIBuffer(ResourceIdentifier* pID) {/**/}

	virtual void OnEnableVBuffer(ResourceIdentifier* pID);
	virtual void OnDisableVBuffer(ResourceIdentifier* pID) {/**/}

	virtual void OnEnableTexture(ResourceIdentifier* pID);
	virtual void OnDisableTexture(ResourceIdentifier* pID) {}  // TODO

	// global render state management
	virtual void SetAlphaState(AlphaState* pState);
	virtual void SetCullState(CullState* pState);
	virtual void SetFogState(FogState* pState);
	virtual void SetWireframeState(WireframeState* pState);
	virtual void SetZBufferState(ZBufferState* pState);

private:
	PdrRendererData* mpData;
};

}

#endif
