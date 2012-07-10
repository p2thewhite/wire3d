// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireEffect.h"
#include "WireImage2D.h"
#include "WireIndexBuffer.h"
#include "WireGXDisplayList.h"
#include "WireGXIndexBuffer.h"
#include "WireGXRendererData.h"
#include "WireGXRendererInput.h"
#include "WireGXVertexBuffer.h"
#include "WireLight.h"
#include "WireMatrix4.h"
#include "WireMesh.h"
#include "WireVertexBuffer.h"
#include <malloc.h>		// for memalign
#include <string.h>		// for memset 

using namespace Wire;

//----------------------------------------------------------------------------
Renderer::Renderer(PdrRendererInput& rInput, UInt width, UInt height,
	Bool, Bool useVSync)
	:
	mTexture2Ds(8, 0),
	mLights(PdrRendererData::MAXLIGHTS, 0),
	mMaxAnisotropy(4.0F),
	mMaxTextureWidth(1024),
	mMaxTextureHeight(1024),
	mIndexBufferMap(1024),
	mVertexBufferMap(1024),
	mTexture2DMap(256),
	mSupportsStaticBatching(false),
	mSupportsDynamicBatching(false)
{
	Initialize(width, height);

	mpData = WIRE_NEW PdrRendererData();
	WIRE_ASSERT(mpData);

	mpData->UseVSync = useVSync;

	VIInit();

	GXRenderModeObj*& rRMode = mpData->RMode;
	rRMode = VIDEO_GetPreferredMode(NULL);

	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
	{
		rRMode->viWidth = 678;
		rRMode->viXOrigin = (VI_MAX_WIDTH_NTSC - 678)/2;
	}

	// allocate 2 framebuffers for double buffering
	mpData->FrameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rRMode));
	mpData->FrameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rRMode));

	// ConfigureMem
	void*& rFifoBuffer = mpData->FifoBuffer;
	rFifoBuffer = memalign(32, PdrRendererData::DEFAULT_FIFO_SIZE);
	memset(rFifoBuffer, 0 , PdrRendererData::DEFAULT_FIFO_SIZE);

	GXInit(rFifoBuffer, PdrRendererData::DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	SetClearColor(rInput.BackgroundColor);

	mTexture2Ds.SetQuantity(8);
	mLights.SetQuantity(PdrRendererData::MAXLIGHTS);

	// InitGX
	f32 yScale = GXGetYScaleFactor(rRMode->efbHeight, rRMode->xfbHeight);
	u32 xfbHeight = GXSetDispCopyYScale(yScale);
	GXSetDispCopySrc(0.0F, 0.0F, rRMode->fbWidth, rRMode->efbHeight);
	GXSetDispCopyDst(rRMode->fbWidth, xfbHeight);
	GXSetCopyFilter(rRMode->aa, rRMode->sample_pattern, GX_TRUE,
		rRMode->vfilter);
	GXSetDispCopyGamma(GX_GM_1_0);

	GXSetFieldMode(rRMode->field_rendering,
		((rRMode->viHeight == 2*rRMode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

	if (rRMode->aa)
	{
		GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	}
	else
	{
		GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
	}

	// Note that following is an appropriate setting for full-frame AA mode.
	// You should use "xfbHeight" instead of "efbHeight" to specify actual
	// view size. Since this library doesn't support such special case, please
	// call these in each application to override the normal setting.
	// 	GXSetViewport(0.0F, 0.0F, (f32)Rmode->fbWidth, (f32)Rmode->xfbHeight, 
	// 		0.0F, 1.0F);
	// 	GXSetDispCopyYScale(1.0F);

	const UInt frameBufferIndex = mpData->FrameBufferIndex;
	GXCopyDisp(mpData->FrameBuffer[frameBufferIndex], GX_TRUE);

	// StartVI
	VIConfigure(rRMode);
	VISetNextFrameBuffer(mpData->FrameBuffer[frameBufferIndex]);
	VISetBlack(FALSE);
	VIFlush();
	VIWaitForRetrace();
	if (rRMode->viTVMode & VI_NON_INTERLACE)
	{
		VIWaitForRetrace();
	}

	mWidth = rRMode->fbWidth;
	mHeight = rRMode->efbHeight;

	// Initialize global render state to default settings.
	Set(State::Default);
}

//----------------------------------------------------------------------------
Renderer::~Renderer()
{
	WIRE_DELETE mpData;
	Terminate();
}

//----------------------------------------------------------------------------
Bool Renderer::PreDraw(Camera* pCamera)
{
	SetCamera(pCamera);

	// Invalidate texture cache in GP
	GXInvalidateTexAll();

	return true;
}

//----------------------------------------------------------------------------
void Renderer::ClearBuffers()
{
	if (mpData->IsFrameBufferDirty)
	{
		GXCopyDisp(mpData->FrameBuffer[(mpData->FrameBufferIndex)^1], GX_TRUE);
	}
}

//----------------------------------------------------------------------------
void Renderer::DisplayBackBuffer()
{
	// Set Z/Color update to make sure eFB will be cleared at GXCopyDisp.
	// (If you want to control these modes by yourself in your application,
	// please comment out this part.)
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetColorUpdate(GX_TRUE);

	// Issue display copy command
	UInt& rFrameBufferIndex = mpData->FrameBufferIndex;
	rFrameBufferIndex ^= 1;		// flip framebuffer
	GXCopyDisp(mpData->FrameBuffer[rFrameBufferIndex], GX_TRUE);
	mpData->IsFrameBufferDirty = false;

	// Wait until everything is drawn and copied into XFB.
	// This stalls until all graphics commands have executed and the
	// last pixel is written to the frame buffer.
	// This should only be used in immediate mode rendering.
	GXDrawDone();

	// Display the buffer which was just filled by GXCopyDisplay
	VISetNextFrameBuffer(mpData->FrameBuffer[rFrameBufferIndex]);

	// Tell VI device driver to write the current VI settings so far
	VIFlush();

	if (mpData->UseVSync)
	{
		VIWaitForRetrace();
	}
}

//----------------------------------------------------------------------------
void Renderer::SetClearColor(const ColorRGBA& rClearColor)
{
	mClearColor = rClearColor;

	GXColor& rGXClearColor = mpData->ClearColor;

	rGXClearColor.r = static_cast<UChar>(rClearColor.R() * 255.0F);
	rGXClearColor.g = static_cast<UChar>(rClearColor.G() * 255.0F);
	rGXClearColor.b = static_cast<UChar>(rClearColor.B() * 255.0F);
	rGXClearColor.a = static_cast<UChar>(rClearColor.A() * 255.0F);
	GXSetCopyClear(rGXClearColor, GX_MAX_Z24);
}

//----------------------------------------------------------------------------
void Renderer::SetWorldTransformation(Transformation& rWorld, Bool
	usesNormals)
{
	Matrix34F model;
	rWorld.GetTransformation(model);
	// load the modelview matrix into matrix memory
	Matrix34F modelView = mpData->ViewMatrix * model;
	GXLoadPosMtxImm(modelView, GX_PNMTX0);

	if (usesNormals)
	{
		Mtx modelViewInverse;
		MTXInverse(modelView, modelViewInverse);
		MTXTranspose(modelViewInverse, modelView);
		GXLoadNrmMtxImm(modelView, GX_PNMTX0);
	}
}

//----------------------------------------------------------------------------
void Renderer::DrawElements(UInt activeIndexCount, UInt indexOffset)
{
	mStatistics.DrawCalls++;
	mStatistics.Triangles += activeIndexCount/3;

	mpData->IsFrameBufferDirty = true;

	GXSetNumChans(1);

	Bool hasTextures = false;
	if (mspMaterial)
	{
		UInt textureCount = mspMaterial->GetTextureQuantity();
		if (textureCount > 0)
		{
			hasTextures = true;
			GXSetNumTexGens(textureCount);
			GXSetNumTevStages(textureCount);
		}
	}

	if (!hasTextures)
	{
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
			GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	}

	WIRE_ASSERT(mspIndexBuffer);
	const IndexBuffer& rIBuffer = *mspIndexBuffer;
	PdrVertexBuffer* pPdrVBuffer = mpData->PdrVBuffer;
	WIRE_ASSERT(pPdrVBuffer);
	PdrIndexBuffer* pPdrIBuffer = mpData->PdrIBuffer;
	WIRE_ASSERT(pPdrIBuffer);

	if (GetStateWireframe() && GetStateWireframe()->Enabled)
	{
		mpData->DrawWireframe(pPdrVBuffer->GetDeclaration(), pPdrIBuffer->
			GetBuffer(), activeIndexCount, indexOffset);
	}
	else
	{
		WIRE_ASSERT(mspVertexBuffer);
		const UInt key = mspVertexBuffer->GetAttributes().GetKey();
		PdrDisplayList** pEntry = pPdrIBuffer->GetDisplayLists().Find(key);
		PdrDisplayList* pDisplayList = NULL;

		Bool isStatic = ((indexOffset == 0) && 
			(activeIndexCount == rIBuffer.GetQuantity()));

		if (pEntry)
		{
			pDisplayList = *pEntry;
		}
		else if ((rIBuffer.GetUsage() == Buffer::UT_STATIC) && isStatic)
		{
			pDisplayList = WIRE_NEW PdrDisplayList(mpData, *pPdrIBuffer,
				rIBuffer.GetQuantity(), pPdrVBuffer->GetDeclaration());
			pPdrIBuffer->GetDisplayLists().Insert(key, pDisplayList);
		}

		if (pDisplayList && isStatic)
		{
			pDisplayList->Draw();
		}
		else
		{
			mpData->Draw(pPdrVBuffer->GetDeclaration(), pPdrIBuffer->
				GetBuffer(), activeIndexCount, indexOffset);
		}
	}
}

//----------------------------------------------------------------------------
void Renderer::DrawElements(UInt vertexCount, UInt activeIndexCount,
	UInt indexOffset)
{
	mStatistics.DrawCalls++;
	mStatistics.Triangles += activeIndexCount/3;

	mpData->IsFrameBufferDirty = true;

	GXSetNumChans(1);

	Bool hasTextures = false;
	if (mspMaterial)
	{
		UInt textureCount = mspMaterial->GetTextureQuantity();
		if (textureCount > 0)
		{
			hasTextures = true;
			GXSetNumTexGens(textureCount);
			GXSetNumTevStages(textureCount);
		}
	}

	if (!hasTextures)
	{
		GXSetNumTexGens(0);
		GXSetNumTevStages(1);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
			GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	}

	PdrVertexBuffer* pPdrVBuffer = mpData->PdrVBuffer;
	WIRE_ASSERT(pPdrVBuffer);
	PdrIndexBuffer* pPdrIBuffer = mpData->PdrIBuffer;
	WIRE_ASSERT(pPdrIBuffer);

	if (GetStateWireframe() && GetStateWireframe()->Enabled)
	{
		mpData->DrawWireframe(pPdrVBuffer->GetDeclaration(), pPdrIBuffer->
			GetBuffer(), activeIndexCount, indexOffset);
	}
	else
	{
		mpData->Draw(pPdrVBuffer->GetDeclaration(), pPdrIBuffer->GetBuffer(),
			activeIndexCount, indexOffset);
	}
}

//----------------------------------------------------------------------------
void Renderer::SetCamera(Camera* pCamera)
{
	if (!pCamera)
	{
		return;
	}

	mspCamera = pCamera;
	OnFrameChange();
	OnViewportChange();

	Mtx44 projection;

	if (pCamera->IsPerspective())
	{
		MTXFrustum(projection, pCamera->GetUMax(), pCamera->GetUMin(),
			pCamera->GetRMin(), pCamera->GetRMax(), pCamera->GetDMin(),
			pCamera->GetDMax());
		GXSetProjection(projection, GX_PERSPECTIVE);
	}
	else
	{
		MTXOrtho(projection, pCamera->GetUMax(), pCamera->GetUMin(),
			pCamera->GetRMin(), pCamera->GetRMax(), pCamera->GetDMin(),
			pCamera->GetDMax());
		GXSetProjection(projection, GX_ORTHOGRAPHIC);
	}
}

//----------------------------------------------------------------------------
void Renderer::OnFrameChange()
{
	WIRE_ASSERT(mspCamera);
	Vector3F eye = mspCamera->GetLocation();
	Vector3F rVector = mspCamera->GetRVector();
	Vector3F uVector = mspCamera->GetUVector();
	Vector3F dVector = -mspCamera->GetDVector();

	Matrix34F& rViewMatrix = mpData->ViewMatrix;
	rViewMatrix[0][0] = rVector[0];
	rViewMatrix[1][0] = uVector[0];
	rViewMatrix[2][0] = dVector[0];
	rViewMatrix[0][1] = rVector[1];
	rViewMatrix[1][1] = uVector[1];
	rViewMatrix[2][1] = dVector[1];
	rViewMatrix[0][2] = rVector[2];
	rViewMatrix[1][2] = uVector[2];
	rViewMatrix[2][2] = dVector[2];
	rViewMatrix[0][3] = -rVector.Dot(eye);
	rViewMatrix[1][3] = -uVector.Dot(eye);
	rViewMatrix[2][3] = -dVector.Dot(eye);
}

//----------------------------------------------------------------------------
void Renderer::OnViewportChange()
{
	WIRE_ASSERT(mspCamera);
	Float left;
	Float right;
	Float top;
	Float bottom;

	// GX defines the full-sized viewport to have origin at the upper
	// left corner of the screen. Wire uses the OpenGL convention that
	// 'bottom' specifies the relative distance from the bottom of the
	// screen. GX needs a specification of relative distance from the
	// top of the screen, which is 1 - 'top'.
	mspCamera->GetViewport(left, right, top, bottom);
	GXRenderModeObj*& rRMode = mpData->RMode;
	Float originX = left * static_cast<Float>(mWidth);
	Float width = (right - left) *  static_cast<Float>(mWidth);
	Float originY = (1.0F - top) * static_cast<Float>(rRMode->xfbHeight);
	Float height = (top - bottom) * static_cast<Float>(rRMode->xfbHeight);

	// Set up viewport (This is inappropriate for full-frame AA.)
	if (rRMode->field_rendering)
	{
		GXSetViewportJitter(originX, originY, width, height, 0.0F, 1.0F,
			VIGetNextField());
	}
	else
	{
		GXSetViewport(originX, originY, width, height, 0.0F, 1.0F);
	}

	GXSetScissor(originX, originY, width, height);
}

//----------------------------------------------------------------------------
PdrRendererData::PdrRendererData()
	:
	FifoBuffer(NULL),
	RMode(NULL),
	PdrVBuffer(NULL),
	PdrIBuffer(NULL),
	FrameBufferIndex(0),
	IsFrameBufferDirty(false),
	UseVSync(true),
	LightsMask(0)
{
	FrameBuffer[0] = NULL;
	FrameBuffer[1] = NULL;
	ClearColor.r = 0;
	ClearColor.g = 0;
	ClearColor.b = 0;
	ClearColor.a = 0xFF;
	ViewMatrix.MakeZero();
}

//----------------------------------------------------------------------------
void PdrRendererData::ConvertRGBA8888ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	UInt gapX = (4 - xCount) * 2;
	UInt gapY = ((4 - yCount) * 2) * 4;

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					*pDst++ = *(pSrc + offset*4 + 3);
					*pDst++ = *(pSrc + offset*4);
				}

				pDst += gapX;
			}

			pDst += gapY;

			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UChar* pSrcTemp = pSrc + offset*4 + 1;
					*pDst++ = *pSrcTemp++;
					*pDst++ = *pSrcTemp;
				}

				pDst += gapX;
			}
		}
	}
}

//----------------------------------------------------------------------------
void PdrRendererData::ConvertRGB888ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	UInt gapX = (4 - xCount) * 2;
	UInt gapY = ((4 - yCount) * 2) * 4;

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					*pDst++ = 0xFF;
					*pDst++ = *(pSrc + offset*3);
				}

				pDst += gapX;
			}

			pDst += gapY;

			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UChar* pSrcTemp = pSrc + offset*3 + 1;

					*pDst++ = *pSrcTemp++;
					*pDst++ = *pSrcTemp;
				}

				pDst += gapX;
			}
		}
	}
}

//----------------------------------------------------------------------------
void PdrRendererData::ConvertRGB565ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					*pDst++ = *(pSrc+offset*2);
					*pDst++ = *(pSrc+offset*2 + 1);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void PdrRendererData::ConvertRGBA4444ToTiles(UChar* pSrc, UShort width,
	UShort height, UChar* pDst)
{
	UInt tilesYCount;
	UInt tilesXCount;
	UShort yCount = height;
	UShort xCount = width;
	GetTileCount(tilesYCount, yCount, tilesXCount, xCount);

	for (UInt ty = 0; ty < tilesYCount; ty++)
	{
		for (UInt tx = 0; tx < tilesXCount; tx++)
		{
			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UShort texel = static_cast<UShort>(*(pSrc + offset*2));
					texel = texel << 8;
					texel |= static_cast<UShort>(*(pSrc + offset*2 + 1));
					UShort texelRGB = (texel & 0xFFF0) >> 4;
					UShort texelA = (texel & 0x0e) << 11;
					texel = texelRGB | texelA;
					*pDst++ = static_cast<UChar>(texel >> 8);
					*pDst++ = static_cast<UChar>(texel);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
UInt PdrRendererData::GetTotalImageMemory(const Image2D* pImage, UInt bpp)
{
	UInt totalMemory = 0;
	for (UInt mipLevel = 0; mipLevel < pImage->GetMipmapCount(); mipLevel++)
	{
		UInt mipMemoryUsed = pImage->GetQuantity(mipLevel) * bpp;
		if (bpp == 4)
		{
			mipMemoryUsed = mipMemoryUsed < 64 ? 64 : mipMemoryUsed;
		}
		else
		{
			mipMemoryUsed = mipMemoryUsed < 32 ? 32 : mipMemoryUsed;
		}

		totalMemory += mipMemoryUsed;
	}

	return totalMemory;
}

//----------------------------------------------------------------------------
void PdrRendererData::GetTileCount(UInt& rTilesYCount, UShort& rHeight,
	UInt& rTilesXCount, UShort& rWidth)
{
	rTilesYCount = rHeight / 4;
	rTilesXCount = rWidth / 4;

	if (rHeight < 4)
	{
		rTilesYCount = 1;
	}
	else
	{
		rHeight = 4;
	}

	if (rWidth < 4)
	{
		rTilesXCount = 1;
	}
	else
	{
		rWidth = 4;
	}
}

//----------------------------------------------------------------------------
void PdrRendererData::Draw(const TArray<PdrVertexBuffer::VertexElement>&
	rElements, const UShort* const pIndices, UInt activeIndexCount,
	UInt indexOffset)
{
	GXBegin(GX_TRIANGLES, GX_VTXFMT0, activeIndexCount);
	for (UInt i = indexOffset; i < (indexOffset+activeIndexCount); i++)
	{
		UShort index = pIndices[i];

		for (UInt j = 0; j < rElements.GetQuantity(); j++)
		{
			switch (rElements[j].Attr)
			{
			case GX_VA_POS:
				GXPosition1x16(index);
				break;

			case GX_VA_NRM:
				GXNormal1x16(index);
				break;

			case GX_VA_CLR0:
			case GX_VA_CLR1:
				GXColor1x16(index);
				break;

			case GX_VA_TEX0:
			case GX_VA_TEX1:
			case GX_VA_TEX2:
			case GX_VA_TEX3:
			case GX_VA_TEX4:
			case GX_VA_TEX5:
			case GX_VA_TEX6:
			case GX_VA_TEX7:
				GXTexCoord1x16(index);
				break;

			default:
				WIRE_ASSERT(false);
			}
		}
	}

	GXEnd();
}

//----------------------------------------------------------------------------
void PdrRendererData::DrawWireframe(const TArray<PdrVertexBuffer::
	VertexElement>& rElements, const UShort* const pIBuffer,
	UInt activeIndexCount, UInt indexOffset)
{
	UInt indexCount = (activeIndexCount / 3) * 3;
	if (indexCount == 0)
	{
		return;
	}

	GXBegin(GX_LINES, GX_VTXFMT0, indexCount * 2);

	for (UInt i = indexOffset; i < (indexOffset+indexCount); i += 3)
	{
		for (UInt k = 0; k < 3; k++)
		{
			UShort index = pIBuffer[i + k];
			for (UInt j = 0; j < rElements.GetQuantity(); j++)
			{
				switch (rElements[j].Attr)
				{
				case GX_VA_POS:
					GXPosition1x16(index);
					break;

				case GX_VA_NRM:
					GXNormal1x16(index);
					break;

				case GX_VA_CLR0:
					GXColor1x16(index);
					break;

				case GX_VA_TEX0:
				case GX_VA_TEX1:
				case GX_VA_TEX2:
				case GX_VA_TEX3:
				case GX_VA_TEX4:
				case GX_VA_TEX5:
				case GX_VA_TEX6:
				case GX_VA_TEX7:
					GXTexCoord1x16(index);
					break;

				default:
					WIRE_ASSERT(false);
				}
			}

			for (UInt j = 0; j < rElements.GetQuantity(); j++)
			{
				index = pIBuffer[i + ((k+1) > 2 ? 0 : k+1)];
				switch (rElements[j].Attr)
				{
				case GX_VA_POS:
					GXPosition1x16(index);
					break;

				case GX_VA_NRM:
					GXNormal1x16(index);
					break;

				case GX_VA_CLR0:
					GXColor1x16(index);
					break;

				case GX_VA_TEX0:
				case GX_VA_TEX1:
				case GX_VA_TEX2:
				case GX_VA_TEX3:
				case GX_VA_TEX4:
				case GX_VA_TEX5:
				case GX_VA_TEX6:
				case GX_VA_TEX7:
					GXTexCoord1x16(index);
					break;

				default:
					WIRE_ASSERT(false);
				}
			}
		}
	}

	GXEnd();
}

//----------------------------------------------------------------------------
void Renderer::PostDraw()
{
	// Reset state cache (state is not preserved outside Begin/EndScene()),
	// and release smart pointers cached by the Renderer.
	ReleaseResources();
	mspCamera = NULL;
}

//----------------------------------------------------------------------------
void Renderer::SetState(StateWireframe* pState)
{
	mspStates[State::WIREFRAME] = pState;
}

//----------------------------------------------------------------------------
void Renderer::Resize(UInt, UInt)
{
	// nothing to do, window resizing not supported
}

// internally used by System::Assert
//----------------------------------------------------------------------------
void* PdrRendererData::GetFramebuffer()
{
	return FrameBuffer[0];
}

//----------------------------------------------------------------------------
void PdrRendererData::SetFramebufferIndex(UInt i)
{
	FrameBufferIndex = i;
}
