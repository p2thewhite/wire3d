#pragma once
#ifndef WIREPDRRENDERERDATA_H
#define WIREPDRRENDERERDATA_H

#include "WireMatrix34.h"
#include "WireGXVertexBuffer.h"

namespace Wire
{

class Image2D;
class IndexBuffer;
class VertexBuffer;

class PdrRendererData
{
public:
	PdrRendererData();

	// convenience functions
	static UInt GetTotalImageMemory(const Image2D* pImage, UInt bpp);

	static void ConvertRGBA8888ToTiles(UChar* pSrc, UShort width,
		UShort height, UChar* pDst);
	static void ConvertRGB888ToTiles(UChar* pSrc, UShort width, UShort height,
		UChar* pDst);
	static void ConvertRGB565ToTiles(UChar* pSrc, UShort width, UShort height,
		UChar* pDst);
	static void ConvertRGBA4444ToTiles(UChar* pSrc, UShort width,
		UShort height, UChar* pDst);
	static void GetTileCount(UInt& rTilesYCount, UShort& rHeight,
		UInt& rTilesXCount, UShort &rWidth);

	// GX rendering specific functions
	void CreateDisplayList(PdrVertexBuffer* pPdrVBuffer, const IndexBuffer&
		rIBuffer);
	void Draw(const TArray<PdrVertexBuffer::VertexElement>& rElements,
		const IndexBuffer& rIBuffer);
	void DrawWireframe(const PdrVertexBuffer* pPdrVBuffer, const IndexBuffer&
		rIBuffer);

	// internally used by System::Assert
	void* GetFramebuffer();
	void SetFramebufferIndex(UInt i);

	// Platform-dependent data
	void* FifoBuffer;
	void* FrameBuffer[2];
	GXRenderModeObj* RMode;

	PdrVertexBuffer* PdrVBuffer;
	PdrIndexBuffer* PdrIBuffer;
	IndexBuffer* IBuffer;

	GXColor ClearColor;

	// Transforms world space to camera space.
	Matrix34F ViewMatrix;

	static const UInt MaxLights = 8;
	UInt FrameBufferIndex;
	Bool IsFrameBufferDirty;

	UChar LightsMask;

	static UChar sAlphaSrcBlend[];
	static UChar sAlphaDstBlend[];
	static UChar sCullType[];
	static UChar sFogDensity[];
	static UChar sZBufferCompare[];
	static UChar sTexMinFilter[];
	static UChar sTexWrapMode[];
	static UChar sTexBlend[];
	static UChar sImage2DFormat[];
};

}

#endif
