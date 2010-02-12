#pragma once
#ifndef WIREDX9RENDERER_H
#define WIREDX9RENDERER_H

#include "WireMatrix4.h"
#include "WireRenderer.h"
#include <d3d9.h>

namespace Wire
{

class /*WIRE_RENDERER_ITEM*/ Dx9Renderer : public Renderer
{

typedef Renderer Parent;

public:
	Dx9Renderer(HWND hWnd, Int width, Int height);
	virtual ~Dx9Renderer();

	virtual Bool BeginScene(Camera* pCamera);
	virtual void EndScene();

	// Apply camera changes to platform specific renderer.
	virtual void OnFrameChange();

	// full window buffer operations
	virtual void ClearBuffers();
	virtual void DisplayBackBuffer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

protected:
	// device management
	void ResetDevice();

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

	virtual void OnEnableVBuffer(ResourceIdentifier* pID);

	virtual void OnEnableTexture(ResourceIdentifier* pID);
	virtual void OnDisableTexture(ResourceIdentifier* pID);

	// global render state management
	virtual void SetAlphaState(AlphaState* pState);
	virtual void SetCullState(CullState* pState);
	virtual void SetFogState(FogState* pState);
	virtual void SetWireframeState(WireframeState* pState);
	virtual void SetZBufferState(ZBufferState* pState);

private:
	void Convert(const VertexBuffer* pSrc, Float* pDst);

	IDirect3D9* mpD3D;
	IDirect3DDevice9* mpD3DDevice;
	D3DPRESENT_PARAMETERS mPresent;
	Bool mSupports32BitIndices;

	static HRESULT msResult;

	Matrix4F mViewMatrix;

	static DWORD msAlphaSrcBlend[];
	static DWORD msAlphaDstBlend[];
	static DWORD msCullType[];
	static DWORD msFogDensity[];
	static DWORD msZBufferCompare[];
	static DWORD msTexMinFilter[];
	static DWORD msTexMipFilter[];
	static DWORD msTexWrapMode[];
	static D3DFORMAT msImageFormat[];
};

}

#endif