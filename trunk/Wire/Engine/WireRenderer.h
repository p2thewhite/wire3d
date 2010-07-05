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
class PdrIndexBuffer;
class PdrVertexBuffer;
class PdrRendererData;
class PdrRendererInput;
class PdrTexture2D;
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

	// The platform-dependent destructor must call this function after doing
	// any of its own work.
	void Terminate();

	// Object drawing
	void DrawScene(VisibleSet& rVisibleSet);
	void Draw(Geometry* pGeometry);

	// Access to the color value used for clearing the back buffer
	const ColorRGBA& GetClearColor() const;

	// Backbuffer parameters
	UInt GetWidth() const;
	UInt GetHeight() const;
	Float GetMaxAnisotropy() const;

	// Render state handling
	AlphaState* GetAlphaState();
	CullState* GetCullState();
	FogState* GetFogState();
	WireframeState* GetWireframeState();
	ZBufferState* GetZBufferState();

	PdrRendererData* GetRendererData();

	// Index buffer management
	PdrIndexBuffer* Bind(const IndexBuffer* pIndexBuffer);
	void Unbind(const IndexBuffer* pIndexBuffer);
	static void UnbindAll(const IndexBuffer* pIndexBuffer);
	void Enable(const IndexBuffer* pIndexBuffer);
	void Disable(const IndexBuffer* pIndexBuffer);
	PdrIndexBuffer* GetResource(const IndexBuffer* pIndexBuffer);

	// Vertex buffer management
	PdrVertexBuffer* Bind(const VertexBuffer* pVertexBuffer);
	void Unbind(const VertexBuffer* pVertexBuffer);
	static void UnbindAll(const VertexBuffer* pVertexBuffer);
	void Enable(const VertexBuffer* pVertexBuffer);
	void Disable(const VertexBuffer* pVertexBuffer);
	PdrVertexBuffer* GetResource(const VertexBuffer* pVertexBuffer);

	// 2D texture management
	PdrTexture2D* Bind(const Texture2D* pTexture);
	void Unbind(const Texture2D* pTexture);
	static void UnbindAll(const Texture2D* pTexture);
	void Enable(Texture2D* pTexture);
	void Disable(Texture2D* pTexture);
	PdrTexture2D* GetResource(const Texture2D* pTexture);

private:
	// Global render state management
	void SetGlobalState(GlobalStatePtr spStates[]);
	void RestoreGlobalState(GlobalStatePtr spStates[]);

	// Support for destructor. Destroy any remaining resources that the
	// application did not explicitly release.
	void DestroyAllIndexBuffers();
 	void DestroyAllTexture2Ds();
	void DestroyAllVertexBuffers();

	// Global render states
	GlobalStatePtr mspStates[GlobalState::MAX_STATE_TYPE];

	// The camera for establishing the view frustum
	Camera* mpCamera;

	// Current Geometry object for drawing
	Geometry* mpGeometry;

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
	
	typedef TMap<const IndexBuffer*, PdrIndexBuffer*> IndexBufferMap;
	typedef TMap<const VertexBuffer*, PdrVertexBuffer*> VertexBufferMap;
	typedef TMap<const Texture2D*, PdrTexture2D*> Texture2DMap;
	IndexBufferMap mIndexBufferMap;
	VertexBufferMap mVertexBufferMap;
	Texture2DMap mTexture2DMap;

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
};

#include "WireRenderer.inl"

}

#endif
