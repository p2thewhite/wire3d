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
	static void Convert(const VertexBuffer* pSrc, TArray<PdrVertexBuffer::
		VertexElement>& rElements);

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

	void CreateDisplayList(PdrVertexBuffer* pPdrVBuffer, const IndexBuffer&
		rIBuffer);
	void Draw(const PdrVertexBuffer* pPdrVBuffer, const IndexBuffer&
		rIBuffer);
	void DrawWireframe(const PdrVertexBuffer* pPdrVBuffer, const IndexBuffer&
		rIBuffer);

	// internally used by System::Assert
	void* GetFramebuffer();
	void SetFramebufferIndex(UInt i);
	GXRenderModeObj* GetRenderMode();

	// Platform-dependent data
	void* mFifoBuffer;
	void* mFrameBuffer[2];

	PdrVertexBuffer* mpPdrVBuffer;
	PdrIndexBuffer* mpPdrIBuffer;

	GXRenderModeObj* mRMode;
	GXColor mGXClearColor;

	// Transforms world space to camera space.
	Matrix34F mViewMatrix;

	UInt mFrameBufferIndex;
	Bool mIsFrameBufferDirty;

	static UChar msAlphaSrcBlend[];
	static UChar msAlphaDstBlend[];
	static UChar msCullType[];
	static UChar msFogDensity[];
	static UChar msZBufferCompare[];
	static UChar msTexMinFilter[];
	static UChar msTexWrapMode[];
	static UChar msImage2DFormat[];

private:
	Renderer* mpRenderer;
};

}

#endif
