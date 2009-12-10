#include "WireGXRenderer.h"
#include "../Foundation/WireMatrix4.h"
#include "WireGXResources.h"
#include <malloc.h>		// for memalign
#include <string.h>		// for memset 

using namespace Wire;

#define DEFAULT_FIFO_SIZE	(256*1024)

//----------------------------------------------------------------------------
GXRenderer::GXRenderer(const ColorRGBA& rClearColor)
	:
	Renderer(0, 0)
{
	VIInit();

	mRmode = VIDEO_GetPreferredMode(NULL);

	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
	{
		mRmode->viWidth = 678;
		mRmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 678)/2;
	}

	// allocate 2 framebuffers for double buffering
	mFrameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mRmode));
	mFrameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mRmode));
	mFrameBufferIndex = 0;

	// ConfigureMem
	mDemoFifoBuffer = memalign(32, DEFAULT_FIFO_SIZE);
	memset(mDemoFifoBuffer,0 , DEFAULT_FIFO_SIZE);

	GXInit(mDemoFifoBuffer, DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	SetClearColor(rClearColor);

	// InitGX
	GXSetViewport(0.0F, 0.0F, mRmode->fbWidth, mRmode->efbHeight, 0.0F, 1.0F);
	GXSetScissor(0.0F, 0.0F, mRmode->fbWidth, mRmode->efbHeight);
	f32 yScale = GXGetYScaleFactor(mRmode->efbHeight, mRmode->xfbHeight);
	u32 xfbHeight = GXSetDispCopyYScale(yScale);
	GXSetDispCopySrc(0.0F, 0.0F, mRmode->fbWidth, mRmode->efbHeight);
	GXSetDispCopyDst(mRmode->fbWidth, xfbHeight);
	GXSetCopyFilter(mRmode->aa, mRmode->sample_pattern, GX_TRUE, mRmode->vfilter);
	GXSetDispCopyGamma(GX_GM_1_0);

	GXSetFieldMode(mRmode->field_rendering,
		((mRmode->viHeight == 2*mRmode->xfbHeight) ? GX_ENABLE:GX_DISABLE));

	if (mRmode->aa)
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

	GXCopyDisp(mFrameBuffer[mFrameBufferIndex], GX_TRUE);
	mIsFrameBufferDirty = false;

	// StartVI
	VIConfigure(mRmode);
	VISetNextFrameBuffer(mFrameBuffer[mFrameBufferIndex]);
	VISetBlack(FALSE);
	VIFlush();
	VIWaitForRetrace();
	if (mRmode->viTVMode & VI_NON_INTERLACE)
	{
		VIWaitForRetrace();
	}

	mWidth = mRmode->viWidth;
	mHeight = mRmode->viHeight;
}

//----------------------------------------------------------------------------
GXRenderer::~GXRenderer()
{
}

//----------------------------------------------------------------------------
Bool GXRenderer::BeginScene(Camera* pCamera)
{
	Parent::BeginScene(pCamera);

	Matrix4F perspective;
	MTXFrustum(perspective, pCamera->GetUMax(), pCamera->GetUMin(),
		pCamera->GetRMin(), pCamera->GetRMax(), pCamera->GetDMin(),
		pCamera->GetDMax());
	GXSetProjection(perspective, GX_PERSPECTIVE);

	// Set up viewport (This is inappropriate for full-frame AA.)
	if (mRmode->field_rendering)
	{
		GXSetViewportJitter(0.0F, 0.0F, static_cast<Float>(mRmode->fbWidth),
			static_cast<Float>(mRmode->efbHeight), 0.0F, 1.0F,
			VIGetNextField());
	}
	else
	{
		GXSetViewport(0.0F, 0.0F, static_cast<Float>(mRmode->fbWidth),
			static_cast<Float>(mRmode->efbHeight), 0.0F, 1.0F);
	}

	// Invalidate texture cache in GP
	GXInvalidateTexAll();

	return true;
}

//----------------------------------------------------------------------------
void GXRenderer::EndScene()
{
}

//----------------------------------------------------------------------------
void GXRenderer::ClearBuffers()
{
	if (mIsFrameBufferDirty)
	{
		GXCopyDisp(mFrameBuffer[mFrameBufferIndex^1], GX_TRUE);
	}
}

//----------------------------------------------------------------------------
void GXRenderer::DisplayBackBuffer()
{
	// Set Z/Color update to make sure eFB will be cleared at GXCopyDisp.
	// (If you want to control these modes by yourself in your application,
	// please comment out this part.)
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetColorUpdate(GX_TRUE);

	// Issue display copy command
	GXCopyDisp(mFrameBuffer[mFrameBufferIndex^1], GX_TRUE);
	mIsFrameBufferDirty = false;

	// Wait until everything is drawn and copied into XFB.
	// This stalls until all graphics commands have executed and the
	// last pixel is written to the frame buffer.
	// This should only be used in immediate mode rendering.
	// TODO: use display lists
	GXDrawDone();

	// Set the next frame buffer
	// SwapBuffers
	mFrameBufferIndex ^= 1;		// flip framebuffer

	// Display the buffer which was just filled by GXCopyDisplay
	VISetNextFrameBuffer(mFrameBuffer[mFrameBufferIndex]);

	// Tell VI device driver to write the current VI settings so far
	VIFlush();

	VIWaitForRetrace();
}

//----------------------------------------------------------------------------
void GXRenderer::SetClearColor(const ColorRGBA& rClearColor)
{
	Parent::SetClearColor(rClearColor);

	mGXClearColor.r = static_cast<UChar>(rClearColor.R() * 255.0F);
	mGXClearColor.g = static_cast<UChar>(rClearColor.G() * 255.0F);
	mGXClearColor.b = static_cast<UChar>(rClearColor.B() * 255.0F);
	mGXClearColor.a = static_cast<UChar>(rClearColor.A() * 255.0F);
	GXSetCopyClear(mGXClearColor, GX_MAX_Z24);
}

//----------------------------------------------------------------------------
void GXRenderer::DrawElements()
{
	mIsFrameBufferDirty = true;

	Matrix34F model;
	mpGeometry->World.GetTransformation(model);
	// load the modelview matrix into matrix memory
	GXLoadPosMtxImm(model * mViewMatrix, GX_PNMTX0);

	const VertexBuffer* pVBuffer = mpGeometry->VBuffer;
	const ResourceIdentifier* pID = pVBuffer->GetIdentifier(0, this);
	WIRE_ASSERT(pID);
	const VBufferID* pResource = static_cast<const VBufferID*>(pID);

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	const TArray<VBufferID::VertexElement>& rElements = *(pResource->
		Elements);
	for (UInt i = 0; i < rElements.GetQuantity(); i++)
	{
		GXSetVtxDesc(rElements[i].Attr, GX_INDEX16);
		GXSetVtxAttrFmt(GX_VTXFMT0, rElements[i].Attr, rElements[i].CompCnt,
			rElements[i].CompType, 0);
		GXSetArray(rElements[i].Attr, rElements[i].Data, rElements[i].Stride);
	}

	GXSetCullMode(GX_CULL_BACK);

	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
		GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	const IndexBuffer& rIBuffer = *(mpGeometry->IBuffer);

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, rIBuffer.GetIndexQuantity());

	for (UInt i = 0; i < rIBuffer.GetIndexQuantity(); i++)
	{
		UInt index = rIBuffer[i];
		GXPosition1x16(static_cast<UShort>(index));
		GXColor1x16(static_cast<UShort>(index));
	}

	GXEnd();
}

//----------------------------------------------------------------------------
void GXRenderer::OnFrameChange()
{
	Vector3F eye = mpCamera->GetLocation();
	Vector3F rVector = mpCamera->GetRVector();
	Vector3F uVector = mpCamera->GetUVector();
	Vector3F dVector = mpCamera->GetDVector();

	mViewMatrix[0][0] = rVector[0];
	mViewMatrix[1][0] = uVector[0];
	mViewMatrix[2][0] = dVector[0];
	mViewMatrix[0][1] = rVector[1];
	mViewMatrix[1][1] = uVector[1];
	mViewMatrix[2][1] = dVector[1];
	mViewMatrix[0][2] = rVector[2];
	mViewMatrix[1][2] = uVector[2];
	mViewMatrix[2][2] = dVector[2];
	mViewMatrix[0][3] = -rVector.Dot(eye);
	mViewMatrix[1][3] = -uVector.Dot(eye);
	mViewMatrix[2][3] = -dVector.Dot(eye);
}
