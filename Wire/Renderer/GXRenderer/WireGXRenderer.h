#pragma once
#ifndef WIREGXRENDERER_H
#define WIREGXRENDERER_H

#include "WireCamera.h"
#include "WireMatrix34.h"
#include "WireRenderer.h"

namespace Wire
{

class VBufferID;
class IBufferID;

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
	virtual void OnDisableIBuffer(ResourceIdentifier* pID) {/**/}

	virtual void OnEnableVBuffer(ResourceIdentifier* pID);
	virtual void OnDisableVBuffer(ResourceIdentifier* pID) {/**/}

	virtual void OnEnableTexture(ResourceIdentifier* pID);
	virtual void OnDisableTexture(ResourceIdentifier* pID) {}  // TODO

	// global render state management
	virtual void SetAlphaState(AlphaState* pState);
	virtual void SetCullState(CullState* pState);
	virtual void SetFogState(FogState* pState);
	virtual void SetZBufferState(ZBufferState* pState);

private:
	void Convert(const VertexBuffer* pSrc, VBufferID* pResource);
	void CreateDisplayList(VBufferID* pResource, const IndexBuffer& rIBuffer);
	void Draw(const VBufferID* pResource, const IndexBuffer& rIBuffer);

	void ConvertRGBA8888ToTiles(UChar* pSrc, UShort width, UShort height,
		UChar* pDst);
	void ConvertRGB888ToTiles(UChar* pSrc, UShort width, UShort height,
		UChar* pDst);
	void ConvertRGB565ToTiles(UChar* pSrc, UShort width, UShort height,
		UChar* pDst);
	void ConvertRGBA4444ToTiles(UChar* pSrc, UShort width, UShort height,
		UChar* pDst);

	GXColor mGXClearColor;

	GXRenderModeObj* mRmode;

	void* mFrameBuffer[2];
	void* mDemoFifoBuffer;
	UInt mFrameBufferIndex;
	Bool mIsFrameBufferDirty;

	VBufferID* mpVBufferID;
	IBufferID* mpIBufferID;

	// Transforms world space to camera space.
	Matrix34F mViewMatrix;

	static UChar msAlphaSrcBlend[];
	static UChar msAlphaDstBlend[];
	static UChar msCullType[];
	static UChar msFogDensity[];
	static UChar msZBufferCompare[];
 	static UChar msTexMinFilter[];
 	static UChar msTexMipFilter[];
 	static UChar msTexWrapMode[];
	static UChar msImageFormat[];
};

}

#endif
