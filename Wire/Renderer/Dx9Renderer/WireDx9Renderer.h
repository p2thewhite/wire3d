#pragma once
#ifndef WIREDX9RENDERER_H
#define WIREDX9RENDERER_H

#include "../../Engine/WireEngine.h"
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

	// full window buffer operations
	virtual void ClearBuffers();
	virtual void DisplayBackBuffer();

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

protected:
	// device management
	void ResetDevice();

	// Resource enabling and disabling.
	virtual void DisableIBuffer() {};

	// Resource loading and releasing (to/from video memory).
	virtual void OnLoadIBuffer(ResourceIdentifier*& rID,
		IndexBuffer* pBuffer);
	virtual void OnReleaseIBuffer(ResourceIdentifier* pID);

	// Resource enabling and disabling.
	virtual void OnEnableIBuffer(ResourceIdentifier* pID);
	virtual void OnDisableIBuffer(ResourceIdentifier*) {};

	LPDIRECT3D9 mpMain;
	LPDIRECT3DDEVICE9 mpDevice;
	D3DPRESENT_PARAMETERS mPresent;
	Bool mSupports32BitIndices;

	static HRESULT msResult;
};

}

#endif
