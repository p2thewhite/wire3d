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
