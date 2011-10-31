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
#include "WireTransformation.h"

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
		Bool isFullscreen, Bool useVSync);
	~Renderer();

	// The platform-dependent constructor must call this function first before
	// doing any of its own work.
	void Initialize(UInt width, UInt height);

	// The platform-dependent destructor must call this function after doing
	// any of its own work.
	void Terminate();

	// Object drawing
	void DrawScene(VisibleSet* rVisibleSet);
	void DrawScene(TArray<VisibleSet*>& rVisibleSets);
	void Draw(Geometry* pGeometry, Bool restoreState = true,
		Bool useEffect = true);

	// Backbuffer functions
	inline UInt GetWidth() const;
	inline UInt GetHeight() const;
	inline const ColorRGBA& GetClearColor() const;

	// Texture sampler functions
	inline Float GetMaxAnisotropy() const;
	inline UInt GetMaxTextureStages() const;

	inline UInt GetMaxLights() const;

	// Resource management.
	//
	// Bind:  Create a resource corresponding to the input object. The
	//    renderer maintains a mapping between the object and the resource.
	//    In most cases, video memory is allocated and a copy is made from
	//    the corresponding system memory of the object. If Enable is called
	//    before Bind, the renderer silently creates a resource rather than
	//    cause an exception.
	//
	// BindAll:  Create the resources corresponding to the input object for
	//    all renderers in the system.
	//
	// Unbind:  Destroy the resource corresponding to the input object. The
	//    renderer removes the object-resource pair from the mapping. The
	//    object maintains its system memory copy and properties, so it can
	//    always be bound again.
	//
	// UnbindAll:  Destroy the resources corresponding to the input object
	//    for all renderers that created a resource from the object. This
	//    function is called in the destructors for the objects.
	//
	// Enable:  The resource is made active during the current draw call but
	//    before the DrawElements call is made.
	//
	// Disable:  The resource is made inactive during the current draw call
	//    but after the DrawElements call is made.
	//
	// Set:  The resource is only enabled, if it is inactive. This is used
	//    instead of Enable/Disable pairs to avoid redundant resource
	//    activation. However changing activated input objects between Set
	//    calls do not propagate their change.
	//
	// Update:  Lock the video memory of the resource, copy the system memory
	//    contents to it, and then unlock the video memory. This is the
	//    recommended way for updating resources (update the system memory and
	//    then call Update).

	// Bind/Unbind all resources of a geometry object or a scene graph
	static void BindAll(const Spatial* pSpatial);
	static void UnbindAll(const Spatial* pSpatial);

	// Index buffer management
	PdrIndexBuffer* Bind(const IndexBuffer* pIndexBuffer);
	void Unbind(const IndexBuffer* pIndexBuffer);
	static void UnbindAll(const IndexBuffer* pIndexBuffer);
	void Enable(const IndexBuffer* pIndexBuffer);
	void Disable(const IndexBuffer* pIndexBuffer);
	void Set(const IndexBuffer* pIndexBuffer);
	PdrIndexBuffer* GetResource(const IndexBuffer* pIndexBuffer);

	// Vertex buffer management
	PdrVertexBuffer* Bind(const VertexBuffer* pVertexBuffer);
	void Unbind(const VertexBuffer* pVertexBuffer);
	static void UnbindAll(const VertexBuffer* pVertexBuffer);
	void Enable(const VertexBuffer* pVertexBuffer);
	void Disable(const VertexBuffer* pVertexBuffer);
	void Set(const VertexBuffer* pIndexBuffer);
	PdrVertexBuffer* GetResource(const VertexBuffer* pVertexBuffer);
	void Update(const VertexBuffer* pVertexBuffer);

	// 2D texture management
	PdrTexture2D* Bind(const Texture2D* pTexture);
	void Unbind(const Texture2D* pTexture);
	static void UnbindAll(const Texture2D* pTexture);
	void Enable(const Texture2D* pTexture, UInt unit = 0);
	void Disable(const Texture2D* pTexture, UInt unit = 0);
	void Set(const Texture2D* pTexture, UInt unit = 0);
	PdrTexture2D* GetResource(const Texture2D* pTexture);

	// Material management
	void Enable(const Material* pMaterial);
	void Disable(const Material* pMaterial);
	void Set(const Material* pMaterial);

	// Platform-dependent portion of the Renderer

	// Support for predraw and postdraw semantics.
	Bool PreDraw(Camera* pCamera = NULL);
	void PostDraw();

	// Release all the smart pointers that the Renderer caches between PreDraw()
	// and PostDraw() calls.
	void ReleaseReferences();

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

	// Immediate render state handling
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
	// Renderer render state handling
	void Set(StatePtr spStates[]);
	void Enable(StatePtr spStates[]);
	void Disable(StatePtr spStates[]);

	// Immediate Light state handling
	void SetLight(const Light* pLight, UInt unit = 0);
	void EnableLighting(const ColorRGB& rAmbient = ColorRGB::BLACK);
	void DisableLighting();
	// Renderer light state handling
	void Enable(const TArray<Pointer<Light> >& rLights);
	void Disable(const TArray<Pointer<Light> >& rLights);
	void Set(const TArray<Pointer<Light> >& rLights);

	struct Statistics
	{
		UInt DrawCalls;
		UInt Triangles;
	};

	inline const Statistics& GetStatistics() const;
	void ResetStatistics();

	inline PdrRendererData* GetRendererData() const;

	enum
	{
		MAX_GLOBAL_EFFECTS = 64
	};

private:
	void SetBlendMode(Material::BlendMode blendMode, UInt unit = 0,
		Bool hasAlpha = true);

	void SetWorldTransformation(Transformation& rWorld);

	// The main entry point to drawing in the derived-class renderers
	void DrawElements(UInt activeIndexCount, UInt indexOffset);

	typedef THashTable<const IndexBuffer*, PdrIndexBuffer*> IndexBufferMap;
	typedef THashTable<const VertexBuffer*, PdrVertexBuffer*> VertexBufferMap;
	typedef THashTable<const Texture2D*, PdrTexture2D*> Texture2DMap;

	// Support for destructor. Destroy any remaining resources that the
	// application did not explicitly release.
	void DestroyAll(IndexBufferMap& rIndexBufferMap);
	void DestroyAll(VertexBufferMap& rVertexBufferMap);
 	void DestroyAll(Texture2DMap& rTexture2DMap);

	// Objects currently in use by the Renderer
	StatePtr mspStates[State::MAX_STATE_TYPE];
	Pointer<IndexBuffer> mspIndexBuffer;
	Pointer<Material> mspMaterial;
	Pointer<VertexBuffer> mspVertexBuffer;
	TArray<Pointer<Texture2D> > mTexture2Ds;
	TArray<Pointer<Light> > mLights;

	// The camera for establishing the view frustum
	Pointer<Camera> mspCamera;

	// The color used for clearing the backbuffer
	ColorRGBA mClearColor;

	// Width and height of the backbuffer
	UInt mWidth;
	UInt mHeight;

	// Maximum anisotropy value supported for texture filtering by the device
	Float mMaxAnisotropy;

	static Renderer* s_pRenderer;

	friend class PdrRendererData;
	PdrRendererData* mpData;
	
	IndexBufferMap mIndexBufferMap;
	VertexBufferMap mVertexBufferMap;
	Texture2DMap mTexture2DMap;

	Statistics mStatistics;
};

#include "WireRenderer.inl"

}

#endif
