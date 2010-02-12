#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "WireAlphaState.h"
#include "WireColorRGBA.h"
#include "WireCullState.h"
#include "WireFogState.h"
#include "WireSmartPointer.h"
#include "WireWireframeState.h"
#include "WireZBufferState.h"

namespace Wire
{

class Bindable;
class Camera;
class Geometry;
class IndexBuffer;
class ResourceIdentifier;
class Texture;
class VertexBuffer;
class VisibleSet;

class /*WIRE_ENGINE_ITEM*/ Renderer
{
public:
	// Abstract API for renderers.  Each graphics API must implement this
	// layer.
	virtual ~Renderer();

	// Support for predraw and postdraw semantics.
	virtual Bool BeginScene(Camera* pCamera);
	virtual void EndScene();

	// Apply camera changes to platform specific renderer.
	virtual void OnFrameChange() = 0;

	// Support for full-sized window buffer operations. The values used for
	// clearing are specified by SetClearColor().
	virtual void ClearBuffers() = 0;
	virtual void DisplayBackBuffer() = 0;

	// The main entry point to drawing in the derived-class renderers.
	virtual void DrawElements() = 0;

	// Object drawing.
	void DrawScene(VisibleSet& rVisibleSet);
	void Draw(Geometry* pGeometry);

	// Access to the color value used for clearing the back buffer.
	virtual void SetClearColor(const ColorRGBA& rClearColor);
	const ColorRGBA& GetClearColor() const;

	// Window parameters.
	Int GetWidth() const;
	Int GetHeight() const;

	// Function pointer types for binding and unbinding resources.
	typedef void (Renderer::*ReleaseFunction)(Bindable*);

	virtual void SetAlphaState(AlphaState* pState);
	virtual void SetCullState(CullState* pState);
	virtual void SetFogState(FogState* pState);
	virtual void SetWireframeState(WireframeState* pState);
	virtual void SetZBufferState(ZBufferState* pState);
	AlphaState* GetAlphaState();
	CullState* GetCullState();
	FogState* GetFogState();
	WireframeState* GetWireframeState();
	ZBufferState* GetZBufferState();

protected:
	Renderer(Int width, Int height);

	// Global render state management.
	void SetGlobalState(GlobalStatePtr spStates[]);
	void RestoreGlobalState(GlobalStatePtr spStates[]);

	// Resource loading and releasing.
	void LoadIBuffer(IndexBuffer* pIBuffer);
	void ReleaseIBuffer(Bindable* pIBuffer);

	void LoadVBuffer(VertexBuffer* pVBuffer);
	void ReleaseVBuffer(Bindable* pVBuffer);

	void LoadTexture(Texture* pTexture);
	void ReleaseTexture(Bindable* pTexture);

	// Resource enabling and disabling.
	virtual void EnableIBuffer();
	ResourceIdentifier* EnableVBuffer();

	void EnableTexture(Texture* pTexture);
 	void DisableTexture(Texture* pTexture);

	// Resource loading and releasing (to/from video memory).
	virtual void OnLoadIBuffer(ResourceIdentifier*& rID,
		IndexBuffer* pBuffer) = 0;
	virtual void OnReleaseIBuffer(ResourceIdentifier* pID) = 0;

 	virtual void OnLoadVBuffer(ResourceIdentifier*& rID,
 		VertexBuffer* pVBuffer) = 0;
	virtual void OnReleaseVBuffer(ResourceIdentifier* pID) = 0;

	virtual void OnLoadTexture(ResourceIdentifier*& rID,
		Texture* pTexture) = 0;
	virtual void OnReleaseTexture(ResourceIdentifier* pID) = 0;

	// Resource enabling and disabling.
	virtual void OnEnableIBuffer(ResourceIdentifier* pID) = 0;

	virtual void OnEnableVBuffer(ResourceIdentifier* pID) = 0;

	virtual void OnEnableTexture(ResourceIdentifier* pID) = 0;
	virtual void OnDisableTexture(ResourceIdentifier* pID) = 0;

	// Global render states.
	GlobalStatePtr mspStates[GlobalState::MAX_STATE_TYPE];

	// Current Geometry object for drawing.
	Geometry* mpGeometry;

	// The camera for establishing the view frustum.
	Camera* mpCamera;

	ColorRGBA mClearColor;

	// Width and height of the backbuffer
	Int mWidth;
	Int mHeight;

	static Float msMaxAnisotropy;
};

#include "WireRenderer.inl"

}

#endif