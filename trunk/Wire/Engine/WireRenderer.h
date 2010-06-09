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
class PdrRendererData;
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

	// Object drawing.
	void DrawScene(VisibleSet& rVisibleSet);
	void Draw(Geometry* pGeometry);

	// Access to the color value used for clearing the back buffer.
	const ColorRGBA& GetClearColor() const;

	// Window parameters.
	Int GetWidth() const;
	Int GetHeight() const;

	// Function pointer types for binding and unbinding resources.
	typedef void (Renderer::*ReleaseFunction)(Bindable*);

	// Render state handling
	AlphaState* GetAlphaState();
	CullState* GetCullState();
	FogState* GetFogState();
	WireframeState* GetWireframeState();
	ZBufferState* GetZBufferState();

	// use by System::Assert on Wii
	PdrRendererData* GetRendererData() { return mpData; }

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
	void EnableIBuffer();
	void DisableIBuffer();

	ResourceIdentifier* EnableVBuffer();
	void DisableVBuffer(ResourceIdentifier* pID);

	void EnableTexture(Texture* pTexture);
 	void DisableTexture(Texture* pTexture);

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

	UInt mCurrentSampler;

	static Float msMaxAnisotropy;

	PdrRendererData* mpData;
	
// Platform-dependent portion of the Renderer
public:
	// Support for predraw and postdraw semantics.
	Bool OnBeginScene(Camera* pCamera);
	Bool BeginScene(Camera* pCamera);
	void EndScene();

	// Apply camera changes to platform specific renderer.
	void OnFrameChange();
	void OnViewportChange();

	// Support for full-sized window buffer operations. The values used for
	// clearing are specified by SetClearColor().
	void ClearBuffers();
	void DisplayBackBuffer();

	// The main entry point to drawing in the derived-class renderers.
	void DrawElements();

	void SetClearColor(const ColorRGBA& rClearColor);

	// Render state handling
	void SetAlphaState(AlphaState* pState);
	void SetCullState(CullState* pState);
	void SetFogState(FogState* pState);
	void SetWireframeState(WireframeState* pState);
	void SetZBufferState(ZBufferState* pState);

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
	virtual void OnDisableIBuffer(ResourceIdentifier* pID) = 0;

	virtual void OnEnableVBuffer(ResourceIdentifier* pID) = 0;
	virtual void OnDisableVBuffer(ResourceIdentifier* pID) = 0;

	virtual void OnEnableTexture(ResourceIdentifier* pID) = 0;
	virtual void OnDisableTexture(ResourceIdentifier* pID) = 0;
};

#include "WireRenderer.inl"

}

#endif
