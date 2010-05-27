#pragma once
#ifndef WIREPDRRENDERERDATA_H
#define WIREPDRRENDERERDATA_H

namespace Wire
{

class VertexBuffer;

class PdrRendererData
{
public:
	PdrRendererData() {}
	~PdrRendererData() {}

	void Convert(const VertexBuffer* pSrc, Float* pDst);

	// Platform-dependent data
	IDirect3D9* mpD3D;
	IDirect3DDevice9* mpD3DDevice;
	D3DPRESENT_PARAMETERS mPresent;
	Bool mSupports32BitIndices;

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
