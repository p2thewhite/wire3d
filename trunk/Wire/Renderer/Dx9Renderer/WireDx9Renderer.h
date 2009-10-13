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

	// The entry point to drawing a geometry object.
	virtual void DrawElements();

	virtual void DisplayBackBuffer();

protected:
	LPDIRECT3D9 mpMain;
	LPDIRECT3DDEVICE9 mpDevice;
	D3DPRESENT_PARAMETERS mPresent;
	Bool mSupports32BitIndices;

	static HRESULT msResult;
};

}

#endif
