// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2013. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRERENDERER_H
#define WIRERENDERER_H

#include "WireColorRGBA.h"
#include "WireMaterial.h"
#include "WireRendererStatistics.h"
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
class IndexBuffer;
class Light;
class Mesh;
class PdrIndexBuffer;
class PdrRendererData;
class PdrRendererInput;
class PdrTexture2D;
class PdrVertexFormat;
class PdrVertexBuffer;
class RenderObject;
class Texture2D;
class VertexAttributes;
class VertexBuffer;
class VisibleSet;

class Renderer
{
public:

	Renderer(PdrRendererInput& rInput, UInt width, UInt height,
		Bool isFullscreen, Bool useVSync);
	~Renderer();

	// draw a single object
	void Draw(const RenderObject* pRenderObject, Bool restoreState = false);
	// draw array of objects [min,max) (using batching if enabled)
	void Draw(RenderObject* const pVisible[], UInt min, UInt max);
	// draw set(s) of objects supporting Wire::Effect
	void Draw(VisibleSet* rVisibleSet);
	void Draw(TArray<VisibleSet*>& rVisibleSets);

	// Support for predraw and postdraw semantics.
	Bool PreDraw(Camera* pCamera = NULL);
	void PostDraw();

	// Batching
	void CreateBatchingBuffers(UInt iboSize, UInt vboSize = 0,
		UInt maxVertexStreams = 6);
	inline Bool SupportsBatching() const;
	inline Bool UsesBatching() const;
	inline UInt GetIndexBatchingThreshold() const;
	inline void SetIndexBatchingThreshold(UInt threshold);
	inline UInt GetVertexBatchingThreshold() const;
	inline void SetVertexBatchingThreshold(UInt threshold);

	// Texture sampler functions
	inline Float GetMaxAnisotropy() const;
	inline UInt GetMaxTextureStages() const;
	inline UInt GetMaxTextureWidth() const;
	inline UInt GetMaxTextureHeight() const;

	inline UInt GetMaxVertexStreams() const;

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

	// Bind/Unbind all resources of a RenderObject
	void Bind(const RenderObject* pRenderObject);
	void Unbind(const RenderObject* pRenderObject);

	// Index buffer management
	PdrIndexBuffer* Bind(const IndexBuffer* pIndexBuffer);
	void Unbind(const IndexBuffer* pIndexBuffer);
	static void UnbindAll(const IndexBuffer* pIndexBuffer);
	void Enable(const IndexBuffer* pIndexBuffer);
	void Disable(const IndexBuffer* pIndexBuffer);
	void Set(const IndexBuffer* pIndexBuffer);
	void Update(const IndexBuffer* pIndexBuffer);
	void Update(const IndexBuffer* pIndexBuffer, UInt count, UInt offset = 0);
	PdrIndexBuffer* GetResource(const IndexBuffer* pIndexBuffer);

	// Vertex buffer management
	PdrVertexBuffer* Bind(const VertexBuffer* pVertexBuffer);
	void Unbind(const VertexBuffer* pVertexBuffer);
	static void UnbindAll(const VertexBuffer* pVertexBuffer);
	void Enable(const VertexBuffer* pVertexBuffer, UInt streamIndex);
	void Disable(const VertexBuffer* pVertexBuffer, UInt streamIndex);
	void Set(const VertexBuffer* pIndexBuffer, UInt streamIndex);
	void Update(const VertexBuffer* pVertexBuffer);
	void Update(const VertexBuffer* pVertexBuffer, UInt count,
		UInt offset = 0);
	PdrVertexBuffer* GetResource(const VertexBuffer* pVertexBuffer);

	// Vertex format management
	PdrVertexFormat* Bind(const TArray<Pointer<VertexBuffer> >&
		rVertexBuffers);
	void Enable(const TArray<Pointer<VertexBuffer> >& rVertexBuffers);
	void Disable(const TArray<Pointer<VertexBuffer> >& rVertexBuffers);
	void Set(const TArray<Pointer<VertexBuffer> >& rVertexBuffers);
	PdrVertexFormat* GetResource(const TArray<Pointer<VertexBuffer> >&
		rVertexBuffers);
	
	// 2D texture management
	PdrTexture2D* Bind(const Image2D* pImage);
	void Unbind(const Image2D* pImage);
	static void UnbindAll(const Image2D* pImage);
	void Enable(const Texture2D* pTexture, UInt unit = 0);
	void Disable(const Texture2D* pTexture, UInt unit = 0);
	void Set(const Texture2D* pTexture, UInt unit = 0);
	PdrTexture2D* GetResource(const Image2D* pTexture);

	// Material management
	void Enable(const Material* pMaterial);
	void Disable(const Material* pMaterial);
	void Set(const Material* pMaterial);

	// Mesh management
	void Bind(const Mesh* pMesh);
	void Unbind(const Mesh* pMesh);
	static void UnbindAll(const Mesh* pMesh);
	void Enable(const Mesh* pMesh);
 	void Disable(const Mesh* pMesh);
 	void Set(const Mesh* pMesh);

	// Release all references to non renderer owned resources (disables all
	// actively used resources in the renderer).
	void ReleaseResources();

	// Apply camera changes to platform specific renderer.
	void SetCamera(Camera* pCamera);
	Camera* GetCamera() const;
	void OnFrameChange();
	void OnViewportChange();

	// Handle window resize.
	void Resize(UInt width, UInt height);

	// Backbuffer functions
	inline UInt GetWidth() const;
	inline UInt GetHeight() const;
	inline const ColorRGBA& GetClearColor() const;
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
	void Set(const StatePtr spStates[]);
	void Enable(const StatePtr spStates[]);
	void Disable(const StatePtr spStates[]);

	// Fixed function immediate light state handling
	void SetLight(const Light* pLight, UInt unit = 0);
	void EnableLighting(const ColorRGB& rAmbient = ColorRGB::BLACK);
	void DisableLighting();

	// Renderer light state handling
	void Enable(const TArray<Pointer<Light> >& rLights);
	void Disable(const TArray<Pointer<Light> >& rLights);
	void Set(const TArray<Pointer<Light> >& rLights);

	// Draw call and bound resources' memory statistics
	inline RendererStatistics* GetStatistics();

	// The platform-dependent constructor must call this function first before
	// doing any of its own work.
	void Initialize(UInt width, UInt height);

	// The platform-dependent destructor must call this function after doing
	// any of its own work.
	void Terminate();

	inline PdrRendererData* GetRendererData() const;

	enum
	{
		MAX_GLOBAL_EFFECTS = 64
	};

private:
	// Fixed function texture stage management
	void EnableTextureStage(Material::BlendMode blendMode, UInt unit = 0,
		Bool hasAlpha = true);
	void DisableTextureStage(UInt unit = 0);

	void SetWorldTransformation(const Transformation& rWorld,
		Bool usesNormals);
	UInt GetVertexFormatKey(const TArray<Pointer<VertexBuffer> >&
		rVertexBuffers);

	void BatchAllAndDraw(RenderObject* const pVisible[], UInt min, UInt max);
	void BatchIndicesAndDraw(RenderObject* const pVisible[], UInt min,
		UInt max);
	void DrawBatch(const Mesh* pMesh, PdrIndexBuffer* const pIBPdr,
		TArray<PdrVertexBuffer*>& rVBsPdr, UShort vertexCount,
		UInt indexCount);
	void DrawBatch(PdrIndexBuffer* const pIBPdr, UShort vertexCount,
		UInt indexCount, Bool hasNormals);

	// The main entry point to drawing in the derived-class renderers
	void DrawElements(UInt vertexCount, UInt indexCount, UInt startIndex);

	typedef THashTable<const IndexBuffer*, PdrIndexBuffer*> IndexBufferMap;
	typedef THashTable<const VertexBuffer*, PdrVertexBuffer*> VertexBufferMap;
	typedef THashTable<const Image2D*, PdrTexture2D*> Image2DMap;
	typedef THashTable<UInt, PdrVertexFormat*> VertexFormatMap;

	// Support for destructor. Destroy any remaining resources that the
	// application did not explicitly release.
	void DestroyAll(IndexBufferMap& rIndexBufferMap);
	void DestroyAll(VertexBufferMap& rVertexBufferMap);
 	void DestroyAll(Image2DMap& rTexture2DMap);
	void DestroyAll(VertexFormatMap& rVertexFormatMap);

	void DestroyBatchingBuffers();

	// Objects currently in use by the Renderer
	StatePtr mspStates[State::MAX_STATE_TYPE];
	Pointer<IndexBuffer> mspIndexBuffer;
	Pointer<Material> mspMaterial;
	Pointer<Mesh> mspMesh;
	TArray<Pointer<VertexBuffer> > mVertexBuffers;
	TArray<TArray<UInt> > mVertexFormatKeys;
	TArray<Pointer<Texture2D> > mTexture2Ds;
	TArray<Pointer<Light> > mLights;
	UInt mVertexFormatKey;

	// The camera for establishing the view frustum
	Pointer<Camera> mspCamera;

	// The color used for clearing the backbuffer
	ColorRGBA mClearColor;

	// Width and height of the backbuffer
	UInt mWidth;
	UInt mHeight;

	// Maximum anisotropy value supported for texture filtering by the device
	Float mMaxAnisotropy;
	UInt mMaxTextureWidth;
	UInt mMaxTextureHeight;

	static Renderer* s_pRenderer;

	friend class PdrRendererData;
	PdrRendererData* mpData;
	
	IndexBufferMap mIndexBufferMap;
	VertexBufferMap mVertexBufferMap;
	Image2DMap mImage2DMap;
	VertexFormatMap mVertexFormatMap;

	// Batching
	PdrIndexBuffer* mBatchedIndexBuffer;
	TArray<PdrVertexBuffer*> mBatchedVertexBuffers;
	TArray<void*> mRawBatchedVertexBuffers;
	UInt mIndexBatchingThreshold;
	UInt mVertexBatchingThreshold;
	Bool mSupportsBatching;

	RendererStatistics mStatistics;
};

#include "WireRenderer.inl"

}

#endif
