// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREPDRRENDERERDATA_H
#define WIREPDRRENDERERDATA_H

#include "WireMatrix4.h"
#include <d3d9.h>

namespace Wire
{

class VertexBuffer;

class PdrRendererData
{
public:
	PdrRendererData(Renderer* pRenderer);
	void ResetDevice();

	// Platform-dependent data
	IDirect3D9* D3D;
	IDirect3DDevice9* D3DDevice;
	D3DPRESENT_PARAMETERS Present;

	Matrix4F ViewMatrix;

 	static const DWORD ALPHA_SRC_BLEND[];
 	static const DWORD ALPHA_DST_BLEND[];
 	static const DWORD CULL_TYPE[];
 	static const DWORD FOG_DENSITY[];
 	static const DWORD ZBUFFER_COMPARE[];
 	static const DWORD TEX_MIN_FILTER[];
 	static const DWORD TEX_MIP_FILTER[];
 	static const DWORD TEX_WRAP_MODE[];
	static const DWORD TEX_BLEND[];
 	static const D3DFORMAT IMAGE2D_FORMAT[];
	static const DWORD BUFFER_LOCKING[];
	static const D3DPOOL POOLS[];
	static const DWORD USAGES[];

	Bool Supports32BitIndices;
	Bool SupportsMinFAniso;
	Bool SupportsMagFAniso;
	Bool IsDeviceLost;

private:
	Renderer* mpRenderer;
};

}

#endif
