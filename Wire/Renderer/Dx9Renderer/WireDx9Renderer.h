#pragma once
#ifndef WIREDX9RENDERER_H
#define WIREDX9RENDERER_H

#include "WireMatrix4.h"
#include "WireRenderer.h"
#include <d3d9.h>

namespace Wire
{

class PdrRendererData;
class PdrRendererInput;

class /*WIRE_RENDERER_ITEM*/ Dx9Renderer : public Renderer
{

public:
	Dx9Renderer(PdrRendererInput& rInput);
	virtual ~Dx9Renderer();

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
	virtual void OnDisableIBuffer (ResourceIdentifier*) {/**/}

	virtual void OnEnableVBuffer(ResourceIdentifier* pID);
	virtual void OnDisableVBuffer(ResourceIdentifier*) {/**/}

	virtual void OnEnableTexture(ResourceIdentifier* pID);
	virtual void OnDisableTexture(ResourceIdentifier* pID);

	// global render state management
	virtual void SetAlphaState(AlphaState* pState);
	virtual void SetCullState(CullState* pState);
	virtual void SetFogState(FogState* pState);
	virtual void SetWireframeState(WireframeState* pState);
	virtual void SetZBufferState(ZBufferState* pState);

private:
	PdrRendererData* mpData;
};

#include "WireDx9Renderer.inl"

}

#endif
