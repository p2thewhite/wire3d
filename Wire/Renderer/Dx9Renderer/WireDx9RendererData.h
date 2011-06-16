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

 	static DWORD sAlphaSrcBlend[];
 	static DWORD sAlphaDstBlend[];
 	static DWORD sCullType[];
 	static DWORD sFogDensity[];
 	static DWORD sZBufferCompare[];
 	static DWORD sTexMinFilter[];
 	static DWORD sTexMipFilter[];
 	static DWORD sTexWrapMode[];
	static DWORD sTexBlend[];
 	static D3DFORMAT sImage2DFormat[];
	static DWORD sBufferLocking[];
	static D3DPOOL sPools[];
	static DWORD sUsages[];

	Bool Supports32BitIndices;
	Bool IsDeviceLost;

private:
	Renderer* mpRenderer;
};

}

#endif
