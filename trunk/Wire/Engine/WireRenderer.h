#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "WireAlphaState.h"
#include "WireColorRGBA.h"
#include "WireCullState.h"
#include "WireFogState.h"
#include "WireSmartPointer.h"
#include "WireTMap.h"
#include "WireWireframeState.h"
#include "WireZBufferState.h"

namespace Wire
{

class Bindable;
class Camera;
class Geometry;
class IndexBuffer;
class PdrRendererData;
class PdrRendererInput;
class PdrTexture2D;
class ResourceIdentifier;
class Texture2D;
class VertexBuffer;
class VisibleSet;

class /*WIRE_ENGINE_ITEM*/ Renderer
{
public:
	Renderer(PdrRendererInput& rInput, UInt width, UInt height);
	~Renderer();

	// The platform-dependent constructor must call this function first before
	// doing any of its own work.
	void Initialize(UInt width, UInt height);

	// Object drawing
	void DrawScene(VisibleSet& rVisibleSet);
	void Draw(Geometry* pGeometry);

	// Access to the color value used for clearing the back buffer
	const ColorRGBA& GetClearColor() const;

	// Backbuffer parameters
	UInt GetWidth() const;
	UInt GetHeight() const;
	Float GetMaxAnisotropy() const;

	// Function pointer types for binding and unbinding resources
	typedef void (Renderer::*ReleaseFunction)(Bindable*);

	// Render state handling
	AlphaState* GetAlphaState();
	CullState* GetCullState();
	FogState* GetFogState();
	WireframeState* GetWireframeState();
	ZBufferState* GetZBufferState();

	PdrRendererData* GetRendererData();

	// 2D texture management.
	PdrTexture2D* Bind(const Texture2D* pTexture);
	void Unbind(const Texture2D* pTexture);
	static void UnbindAll(const Texture2D* pTexture);
	PdrTexture2D* GetResource(const Texture2D* PdrTexture2D);

protected:
	// Global render state management
	void SetGlobalState(GlobalStatePtr spStates[]);
	void RestoreGlobalState(GlobalStatePtr spStates[]);

	// Resource loading and releasing
	void LoadIBuffer(IndexBuffer* pIBuffer);
	void ReleaseIBuffer(Bindable* pIBuffer);

	void LoadVBuffer(VertexBuffer* pVBuffer);
	void ReleaseVBuffer(Bindable* pVBuffer);

	// Resource enabling and disabling
	void EnableIBuffer();
	void DisableIBuffer();

	ResourceIdentifier* EnableVBuffer();
	void DisableVBuffer(ResourceIdentifier* pID);

	void EnableTexture(Texture2D* pTexture);
 	void DisableTexture(Texture2D* pTexture);

	// Global render states
	GlobalStatePtr mspStates[GlobalState::MAX_STATE_TYPE];

	// Current Geometry object for drawing
	Geometry* mpGeometry;

	// The camera for establishing the view frustum
	Camera* mpCamera;

	// The color used for clearing the backbuffer
	ColorRGBA mClearColor;

	// Width and height of the backbuffer
	UInt mWidth;
	UInt mHeight;

	// Maximum anisotropy value supported for texture filtering by the device
	Float mMaxAnisotropy;

	UInt mCurrentSampler;

	static Renderer* smRenderer;
	PdrRendererData* mpData;
	
	TMap<const Texture2D*, PdrTexture2D*> mTexture2DMap;

	// Platform-dependent portion of the Renderer
public:
	// Support for predraw and postdraw semantics
	Bool PreDraw(Camera* pCamera);
	void PostDraw();

	// Apply camera changes to platform specific renderer.
	void OnFrameChange();
	void OnViewportChange();

	// Support for full-sized window buffer operations. The values used for
	// clearing are specified by SetClearColor().
	void ClearBuffers();
	void DisplayBackBuffer();

	// The main entry point to drawing in the derived-class renderers
	void DrawElements();

	void SetClearColor(const ColorRGBA& rClearColor);

	// Render state handling
	void SetAlphaState(AlphaState* pState);
	void SetCullState(CullState* pState);
	void SetFogState(FogState* pState);
	void SetWireframeState(WireframeState* pState);
	void SetZBufferState(ZBufferState* pState);

	// Resource loading and releasing (to/from video memory)
	void OnLoadIBuffer(ResourceIdentifier*& rID, IndexBuffer* pBuffer);
	void OnReleaseIBuffer(ResourceIdentifier* pID);

	void OnLoadVBuffer(ResourceIdentifier*& rID, VertexBuffer* pVBuffer);
	void OnReleaseVBuffer(ResourceIdentifier* pID);

	void OnLoadTexture(ResourceIdentifier*& rID, Texture2D* pTexture);
	void OnReleaseTexture(ResourceIdentifier* pID);

	// Resource enabling and disabling
	void OnEnableIBuffer(ResourceIdentifier* pID);
	void OnDisableIBuffer(ResourceIdentifier* pID);

	void OnEnableVBuffer(ResourceIdentifier* pID);
	void OnDisableVBuffer(ResourceIdentifier* pID);

	void OnEnableTexture(ResourceIdentifier* pID);
	void OnDisableTexture(ResourceIdentifier* pID);
};

#include "WireRenderer.inl"

}

#endif
