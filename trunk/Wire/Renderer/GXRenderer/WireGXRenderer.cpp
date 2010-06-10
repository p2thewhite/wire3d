#include "WireRenderer.h"

#include "WireCamera.h"
#include "WireGeometry.h"
#include "WireImage.h"
#include "WireGXRendererData.h"
#include "WireGXRendererInput.h"
#include "WireGXResources.h"
#include "WireMatrix4.h"
#include <malloc.h>		// for memalign
#include <string.h>		// for memset 

using namespace Wire;

#define DEFAULT_FIFO_SIZE	(256*1024)

//----------------------------------------------------------------------------
Renderer::Renderer(PdrRendererInput& rInput, UInt width, UInt height)
{
	Initialize(width, height);
	msMaxAnisotropy = 4.0F;

	mpData = WIRE_NEW PdrRendererData();
	WIRE_ASSERT(mpData);

	VIInit();

	GXRenderModeObj*& rRMode = mpData->mRMode;
	rRMode = VIDEO_GetPreferredMode(NULL);

	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
	{
		rRMode->viWidth = 678;
		rRMode->viXOrigin = (VI_MAX_WIDTH_NTSC - 678)/2;
	}

	// allocate 2 framebuffers for double buffering
	mpData->mFrameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rRMode));
	mpData->mFrameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rRMode));

	UInt& rFrameBufferIndex = mpData->mFrameBufferIndex;
	rFrameBufferIndex = 0;

	// ConfigureMem
	void*& rFifoBuffer = mpData->mFifoBuffer;
	rFifoBuffer = memalign(32, DEFAULT_FIFO_SIZE);
	memset(rFifoBuffer,0 , DEFAULT_FIFO_SIZE);

	GXInit(rFifoBuffer, DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	SetClearColor(rInput.BackgroundColor);

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

	GXCopyDisp(mpData->mFrameBuffer[rFrameBufferIndex], GX_TRUE);
	mpData->mIsFrameBufferDirty = false;

	// StartVI
	VIConfigure(rRMode);
	VISetNextFrameBuffer(mpData->mFrameBuffer[rFrameBufferIndex]);
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
	SetGlobalState(GlobalState::Default);
}

//----------------------------------------------------------------------------
Renderer::~Renderer()
{
	WIRE_DELETE mpData;
}

//----------------------------------------------------------------------------
Bool Renderer::BeginScene(Camera* pCamera)
{
	OnBeginScene(pCamera);

	Matrix4F perspective;
	MTXFrustum(perspective, pCamera->GetUMax(), pCamera->GetUMin(),
		pCamera->GetRMin(), pCamera->GetRMax(), pCamera->GetDMin(),
		pCamera->GetDMax());
	GXSetProjection(perspective, GX_PERSPECTIVE);

	// Invalidate texture cache in GP
	GXInvalidateTexAll();

	return true;
}

//----------------------------------------------------------------------------
void Renderer::ClearBuffers()
{
	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	System::Print("\x1b[4;0H");

	if (mpData->mIsFrameBufferDirty)
	{
		GXCopyDisp(mpData->mFrameBuffer[(mpData->mFrameBufferIndex)^1], GX_TRUE);
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
	UInt& rFrameBufferIndex = mpData->mFrameBufferIndex;
	rFrameBufferIndex ^= 1;		// flip framebuffer
	GXCopyDisp(mpData->mFrameBuffer[rFrameBufferIndex], GX_TRUE);
	mpData->mIsFrameBufferDirty = false;

	// Wait until everything is drawn and copied into XFB.
	// This stalls until all graphics commands have executed and the
	// last pixel is written to the frame buffer.
	// This should only be used in immediate mode rendering.
	GXDrawDone();

	// Display the buffer which was just filled by GXCopyDisplay
	VISetNextFrameBuffer(mpData->mFrameBuffer[rFrameBufferIndex]);

	// Tell VI device driver to write the current VI settings so far
	VIFlush();

	VIWaitForRetrace();
}

//----------------------------------------------------------------------------
void Renderer::SetClearColor(const ColorRGBA& rClearColor)
{
	mClearColor = rClearColor;

	GXColor& rGXClearColor = mpData->mGXClearColor;

	rGXClearColor.r = static_cast<UChar>(rClearColor.R() * 255.0F);
	rGXClearColor.g = static_cast<UChar>(rClearColor.G() * 255.0F);
	rGXClearColor.b = static_cast<UChar>(rClearColor.B() * 255.0F);
	rGXClearColor.a = static_cast<UChar>(rClearColor.A() * 255.0F);
	GXSetCopyClear(rGXClearColor, GX_MAX_Z24);
}

//----------------------------------------------------------------------------
void Renderer::DrawElements()
{
	mpData->mIsFrameBufferDirty = true;

	Matrix34F model;
	mpGeometry->World.GetTransformation(model);
	// load the modelview matrix into matrix memory
	GXLoadPosMtxImm(mpData->mViewMatrix * model, GX_PNMTX0);

	GXSetNumChans(1);
//	GXSetNumTexGens(0);
// 	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
// 		GX_COLOR0A0);
// 	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	const IndexBuffer& rIBuffer = *(mpGeometry->IBuffer);

	const WireframeState* pWireframeState = GetWireframeState();
	VBufferID*& rVBufferID = mpData->mpVBufferID;

	if (pWireframeState && pWireframeState->Enabled)
	{
		mpData->DrawWireframe(rVBufferID, rIBuffer);
	}
	else
	{
		TArray<VBufferID::DisplayList>& rDisplayLists = rVBufferID->
			DisplayLists;
		Bool foundDL = false;
		for (UInt i = 0; i < rDisplayLists.GetQuantity(); i++)
		{
			if (rDisplayLists[i].RegisteredIBuffer == mpData->mpIBufferID)
			{
				foundDL = true;
				GXCallDisplayList(rDisplayLists[i].DL, rDisplayLists[i].
					DLSize);
			}
		}

		if (!foundDL)
		{
			mpData->Draw(rVBufferID, rIBuffer);
		}
	}
}

//----------------------------------------------------------------------------
void Renderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	Matrix34F& rViewMatrix = mpData->mViewMatrix;
	rViewMatrix[0][0] = rVector[0];
	rViewMatrix[1][0] = uVector[0];
	rViewMatrix[2][0] = dVector[0];
	rViewMatrix[0][1] = rVector[1];
	rViewMatrix[1][1] = uVector[1];
	rViewMatrix[2][1] = dVector[1];
	rViewMatrix[0][2] = rVector[2];
	rViewMatrix[1][2] = uVector[2];
	rViewMatrix[2][2] = -dVector[2];
	rViewMatrix[0][3] = -rVector.Dot(eye);
	rViewMatrix[1][3] = -uVector.Dot(eye);
	rViewMatrix[2][3] = -dVector.Dot(eye);
}

//----------------------------------------------------------------------------
void Renderer::OnViewportChange()
{
	Float left;
	Float right;
	Float top;
	Float bottom;

	// GX defines the full-sized viewport to have origin at the upper
	// left corner of the screen. Wire uses the OpenGL convention that
	// 'bottom' specifies the relative distance from the bottom of the
	// screen. GX needs a specification of relative distance from the
	// top of the screen, which is 1 - 'top'.
	mpCamera->GetViewport(left, right, top, bottom);
	GXRenderModeObj*& rRMode = mpData->mRMode;
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
void PdrRendererData::Convert(const VertexBuffer* pSrc, VBufferID* pResource)
{
	const VertexAttributes& rIAttr = pSrc->GetAttributes();
	TArray<VBufferID::VertexElement>& rElements = pResource->Elements;

	for (UInt i = 0; i < pSrc->GetVertexQuantity(); i++)
	{
		UInt index = 0;

		if (rIAttr.GetPositionChannels() > 0)
		{
			const Float* pPosition = pSrc->GetPosition(i);
			Float* pDst = static_cast<Float*>(rElements[index++].Data);
			UInt channelCount = rIAttr.GetPositionChannels();

			for (UInt k = 0; k < channelCount; k++)
			{
				pDst[i*channelCount+k] = pPosition[k];
			}
		}

		UInt colorChannelQuantity = rIAttr.GetColorChannelQuantity();
		for (UInt unit = 0; unit < colorChannelQuantity; unit++)
		{
			if (rIAttr.GetColorChannels(unit) > 0)
			{
				const Float* pColor = pSrc->GetColor(i, unit);
				UInt color = 0xFFFFFFFF;
				for (UInt k = 0; k < rIAttr.GetColorChannels(unit); k++)
				{
					color = color << 8;
					color |= static_cast<UChar>(pColor[k] * 255.0F);
				}

				if (rIAttr.GetColorChannels(unit) == 3)
				{
					color = color << 8;
					color |= 0xFF;
				}

				UInt* pDst = static_cast<UInt*>(rElements[index++].Data);
				pDst[i] = color;
			}
		}

		UInt tChannelQuantity = rIAttr.GetTCoordChannelQuantity();
		for (UInt unit = 0; unit < tChannelQuantity; unit++)
		{
			if (rIAttr.GetTCoordChannels(unit) > 0)
			{
				const Float* pUv = pSrc->GetTCoord(i, unit);
				Float* pDst = static_cast<Float*>(rElements[index++].Data);
				UInt channelCount = rIAttr.GetTCoordChannels(unit);

				for (UInt k = 0; k < channelCount; k++)
				{
					pDst[i*channelCount+k] = pUv[k];
				}
			}
		}
	}
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
			}

			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UChar* pSrcTemp = pSrc + offset*4 + 1;
					*pDst++ = *pSrcTemp++;
					*pDst++ = *pSrcTemp;
				}
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
			}

			for (UInt y = 0; y < yCount; y++)
			{
				for (UInt x = 0; x < xCount; x++)
				{
					UInt offset = (ty*width+tx)*4+y*width+x;
					UChar* pSrcTemp = pSrc + offset*3 + 1;

					*pDst++ = *pSrcTemp++;
					*pDst++ = *pSrcTemp;
				}
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
UInt PdrRendererData::GetTotalImageMemory(const Image* pImage, const UInt bpp)
	const
{
	UInt totalMemory = 0;
	for (UInt mipLevel = 0; mipLevel < pImage->GetMipmapCount(); mipLevel++)
	{
		UInt mipMemoryUsed = pImage->GetQuantity(mipLevel) * bpp;
		mipMemoryUsed = mipMemoryUsed < 32 ? 32 : mipMemoryUsed;
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
void PdrRendererData::CreateDisplayList(VBufferID* pResource,
	const IndexBuffer& rIBuffer)
{
	VBufferID::DisplayList DL;

	// Note that the display-list buffer area must be forced out of
	// the CPU cache since it will be written using the write-gather pipe
	const UInt tempDLSize = 65536;
	void* pTempDL = memalign(32, tempDLSize);
	DCInvalidateRange(pTempDL, tempDLSize);

	GXBeginDisplayList(pTempDL, tempDLSize);
	Draw(pResource, rIBuffer);

	DL.DLSize = GXEndDisplayList();
	DL.DL = memalign(32, DL.DLSize);
	System::Memcpy(DL.DL, DL.DLSize, pTempDL, DL.DLSize);
	free(pTempDL);

	DCFlushRange(DL.DL, DL.DLSize);

	IBufferID*& rIBufferID = mpIBufferID;
	DL.RegisteredIBuffer = rIBufferID;
	rIBufferID->RegisteredVBuffers.Append(pResource);
	pResource->DisplayLists.Append(DL);
}

//----------------------------------------------------------------------------
void PdrRendererData::Draw(const VBufferID* pResource,
	const IndexBuffer& rIBuffer)
{
	const TArray<VBufferID::VertexElement>& rElements = pResource->Elements;

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, rIBuffer.GetIndexQuantity());
	for (UInt i = 0; i < rIBuffer.GetIndexQuantity(); i++)
	{
		UShort index = static_cast<UShort>(rIBuffer[i]);

		for (UInt j = 0; j < rElements.GetQuantity(); j++)
		{
			switch (rElements[j].Attr)
			{
			case GX_VA_POS:
				GXPosition1x16(index);
				break;

			case GX_VA_CLR0:
				GXColor1x16(index);
				break;

			case GX_VA_TEX0:
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
void PdrRendererData::DrawWireframe(const VBufferID* pResource,
	const IndexBuffer& rIBuffer)
{
	const TArray<VBufferID::VertexElement>& rElements = pResource->Elements;

	GXBegin(GX_LINES, GX_VTXFMT0, rIBuffer.GetIndexQuantity() * 2);

	for (UInt i = 0; i < rIBuffer.GetIndexQuantity(); i += 3)
	{
		for (UInt k = 0; k < 3; k++)
		{
			UShort index = rIBuffer[i + k];
			for (UInt j = 0; j < rElements.GetQuantity(); j++)
			{
				switch (rElements[j].Attr)
				{
				case GX_VA_POS:
					GXPosition1x16(index);
					break;

				case GX_VA_CLR0:
					GXColor1x16(index);
					break;

				case GX_VA_TEX0:
					GXTexCoord1x16(index);
					break;

				default:
					WIRE_ASSERT(false);
				}		
			}

			for (UInt j = 0; j < rElements.GetQuantity(); j++)
			{
				index = rIBuffer[i + ((k+1) > 2 ? 0 : k)];
				switch (rElements[j].Attr)
				{
				case GX_VA_POS:
					GXPosition1x16(index);
					break;

				case GX_VA_CLR0:
					GXColor1x16(index);
					break;

				case GX_VA_TEX0:
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
void Renderer::EndScene()
{
}

//----------------------------------------------------------------------------
void Renderer::SetWireframeState(WireframeState* pState)
{
	mspStates[GlobalState::WIREFRAME] = pState;
}

// internally used by System::Assert
//----------------------------------------------------------------------------
void* PdrRendererData::GetFramebuffer()
{
	return mFrameBuffer[0];
}

//----------------------------------------------------------------------------
void PdrRendererData::SetFramebufferIndex(UInt i)
{
	mFrameBufferIndex = i;
}

//----------------------------------------------------------------------------
GXRenderModeObj* PdrRendererData::GetRenderMode()
{
	return mRMode;
}

