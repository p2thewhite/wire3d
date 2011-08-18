// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "WireColorRGBA.h"
#include "WireMaterial.h"
#include "WireSmartPointer.h"
#include "WireStateAlpha.h"
#include "WireStateCull.h"
#include "WireStateFog.h"
#include "WireStateMaterial.h"
#include "WireStateWireframe.h"
#include "WireStateZBuffer.h"
#include "WireTHashTable.h"

namespace Wire
{

class Camera;
class Geometry;
class IndexBuffer;
class Light;
class PdrIndexBuffer;
class PdrVertexBuffer;
class PdrRendererData;
class PdrRendererInput;
class PdrTexture2D;
class Spatial;
class Texture2D;
class VertexBuffer;
class VisibleSet;

class Renderer
{
public:
	Renderer(PdrRendererInput& rInput, UInt width, UInt height,
		Bool isFullscreen);
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

	// Backbuffer functions
	inline UInt GetWidth() const;
	inline UInt GetHeight() const;
	inline const ColorRGBA& GetClearColor() const;

	// Texture sampler functions
	inline Float GetMaxAnisotropy() const;
	inline UInt GetMaxTextureStages() const;

	// Bind/Unbind all resources of a geometry object or a scene graph
	static void BindAll(const Spatial* pSpatial);
	static void UnbindAll(const Spatial* pSpatial);

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
	void Update(const VertexBuffer* pVertexBuffer);

	// 2D texture management
	PdrTexture2D* Bind(const Texture2D* pTexture);
	void Unbind(const Texture2D* pTexture);
	static void UnbindAll(const Texture2D* pTexture);
	void Enable(Texture2D* pTexture, UInt unit = 0);
	void Disable(Texture2D* pTexture, UInt unit = 0);
	PdrTexture2D* GetResource(const Texture2D* pTexture);

	// Material management
	void Enable(Material* pMaterial);
	void Disable(Material* pMaterial);

	// Platform-dependent portion of the Renderer

	// Support for predraw and postdraw semantics.
	Bool PreDraw(Camera* pCamera = NULL);
	void PostDraw();

	// Apply camera changes to platform specific renderer.
	void SetCamera(Camera* pCamera);
	void OnFrameChange();
	void OnViewportChange();

	// Handle window resize.
	void Resize(UInt width, UInt height);

	// Support for full-sized window buffer operations.
	void SetClearColor(const ColorRGBA& rClearColor);
	void ClearBuffers();
	void DisplayBackBuffer();

	// Render state handling
	void SetState(StateAlpha* pState);
	void SetState(StateCull* pState);
	void SetState(StateFog* pState);
	void SetState(StateMaterial* pState);
	void SetState(StateWireframe* pState);
	void SetState(StateZBuffer* pState);
	inline const StateAlpha* GetStateAlpha() const;
	inline const StateCull* GetStateCull() const;
	inline const StateFog* GetStateFog() const;
	inline const StateMaterial* GetStateMaterial() const;
	inline const StateWireframe* GetStateWireframe() const;
	inline const StateZBuffer* GetStateZBuffer() const;

	// Light state handling
	void SetLight(const Light* pLight, UInt unit = 0);
	void Enable(const TArray<Pointer<Light> >& rLights);
	void Disable(const TArray<Pointer<Light> >& rLights);
	void EnableLighting();
	void DisableLighting();

	inline PdrRendererData* GetRendererData() const;

private:
	void SetBlendMode(Material::BlendMode blendMode, UInt unit = 0,
		Bool hasAlpha = true);

	// The main entry point to drawing in the derived-class renderers
	void DrawElements(Geometry* pGeometry);

	// Global render state management
	void SetStates(StatePtr spStates[]);
	void RestoreStates(StatePtr spStates[]);

	// Support for destructor. Destroy any remaining resources that the
	// application did not explicitly release.
	void DestroyAllIndexBuffers();
 	void DestroyAllTexture2Ds();
	void DestroyAllVertexBuffers();

	// Global render states
	StatePtr mspStates[State::MAX_STATE_TYPE];

	// The camera for establishing the view frustum
	Camera* mpCamera;

	// The color used for clearing the backbuffer
	ColorRGBA mClearColor;

	// Width and height of the backbuffer
	UInt mWidth;
	UInt mHeight;

	// Maximum anisotropy value supported for texture filtering by the device
	Float mMaxAnisotropy;
	UInt mMaxTextureStages;
	UInt mMaxLights;

	static Renderer* s_pRenderer;

	friend class PdrRendererData;
	PdrRendererData* mpData;
	
	typedef THashTable<const IndexBuffer*, PdrIndexBuffer*> IndexBufferMap;
	typedef THashTable<const VertexBuffer*, PdrVertexBuffer*> VertexBufferMap;
	typedef THashTable<const Texture2D*, PdrTexture2D*> Texture2DMap;
	IndexBufferMap mIndexBufferMap;
	VertexBufferMap mVertexBufferMap;
	Texture2DMap mTexture2DMap;
};

#include "WireRenderer.inl"

}

#endif
