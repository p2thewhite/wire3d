// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREPDRRENDERERDATA_H
#define WIREPDRRENDERERDATA_H

#include "WireMatrix4.h"
#include "WireTArray.h"
#include <d3d9.h>

namespace Wire
{

class Renderer;

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

	struct StateAlpha
	{
		StateAlpha() : IsValid(false) {}
		static const DWORD ALPHA_SRC_BLEND[];
		static const DWORD ALPHA_DST_BLEND[];	
		DWORD SRCBLEND;
		DWORD DESTBLEND;
		DWORD BLENDFACTOR;
		DWORD ALPHABLENDENABLE;
		Bool IsValid;
	} AlphaState;

	struct StateCull
	{
		StateCull() : IsValid(false) {}
		static const DWORD CULL_TYPE[];
		DWORD CULLMODE;
		Bool IsValid;
	} CullState;

	struct StateFog
	{
		StateFog() : IsValid(false) {}
		static const DWORD FOG_DENSITY[];
		DWORD FOGENABLE;
		DWORD FOGSTART;
		DWORD FOGEND;
		DWORD FOGCOLOR;
		DWORD FOGDENSITY;
		DWORD FOGVERTEXMODE;
		Bool IsValid;
	} FogState;

	struct StateTextureStage
	{
		StateTextureStage() : IsValid(false) {}
		DWORD COLOROP;
		DWORD COLORARG0;
		DWORD COLORARG1;
		DWORD COLORARG2;
		DWORD ALPHAOP;
		DWORD ALPHAARG0;
		DWORD ALPHAARG1;
		DWORD ALPHAARG2;
		Bool IsValid;
	};
	TArray<StateTextureStage> TextureStageStates;

	struct StateWireframe
	{
		StateWireframe() : IsValid(false) {}
		DWORD FILLMODE;
		Bool IsValid;
	} WireframeState;

	struct StateZBuffer
	{
		StateZBuffer() : IsValid(false) {}
		static const DWORD ZBUFFER_COMPARE[];
		DWORD ZFUNC;
		DWORD ZWRITEENABLE;
		Bool IsValid;
	} ZBufferState;

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
	Bool UsesRenormalizeNormals;

private:
	Renderer* mpRenderer;
};

}

#endif
