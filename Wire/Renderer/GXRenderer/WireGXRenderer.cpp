#include "WireGXRenderer.h"
#include "../Foundation/WireMatrix4.h"
#include <malloc.h>		// for memalign
#include <string.h>		// for memset 

using namespace Wire;

#define DEFAULT_FIFO_SIZE	(256*1024)

//----------------------------------------------------------------------------
GXRenderer::GXRenderer()
{
	mFrameBuffer[0] = NULL;
	mFrameBuffer[1] = NULL;

	mFrameBufferIndex = 0;

	VIInit();
	PADInit();

	mRmode = VIDEO_GetPreferredMode(NULL);

	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
	{
		mRmode->viWidth = 678;
		mRmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 678)/2;
	}

	// allocate 2 framebuffers for double buffering
	mFrameBuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mRmode));
	mFrameBuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mRmode));

	// ConfigureMem
	mDemoFifoBuffer = memalign(32, DEFAULT_FIFO_SIZE);
	memset(mDemoFifoBuffer,0 , DEFAULT_FIFO_SIZE);

	GXInit(mDemoFifoBuffer, DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	GXColor background = {0, 0, 0, 0xff};
	GXSetCopyClear(background, GX_MAX_Z24);

	// InitGX
	GXSetViewport(0.0f, 0.0f, mRmode->fbWidth, mRmode->efbHeight, 0.0f, 1.0f);
	GXSetScissor(0.0f, 0.0f, mRmode->fbWidth, mRmode->efbHeight);
	f32 yScale = GXGetYScaleFactor(mRmode->efbHeight, mRmode->xfbHeight);
	u32 xfbHeight = GXSetDispCopyYScale(yScale);
	GXSetDispCopySrc(0.0f, 0.0f, mRmode->fbWidth, mRmode->efbHeight);
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
	// 	GXSetViewport(0.0f, 0.0f, (f32)Rmode->fbWidth, (f32)Rmode->xfbHeight, 
	// 		0.0f, 1.0f);
	// 	GXSetDispCopyYScale(1.0f);

	GXCopyDisp(mFrameBuffer[mFrameBufferIndex], GX_TRUE);

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
}

//----------------------------------------------------------------------------
GXRenderer::~GXRenderer()
{
}

//----------------------------------------------------------------------------
Bool GXRenderer::BeginScene(Camera* pCamera)
{
	Parent::BeginScene(pCamera);

	Matrix4f perspective;
	MTXFrustum(perspective, pCamera->GetUMax(), pCamera->GetUMin(),
		pCamera->GetRMin(), pCamera->GetRMax(), pCamera->GetDMin(),
		pCamera->GetDMax());
	GXSetProjection(perspective, GX_PERSPECTIVE);

	// Set up viewport (This is inappropriate for full-frame AA.)
	if (mRmode->field_rendering)
	{
		GXSetViewportJitter(0.0f, 0.0f, static_cast<Float>(mRmode->fbWidth),
			static_cast<Float>(mRmode->efbHeight), 0.0f, 1.0f,
			VIGetNextField());
	}
	else
	{
		GXSetViewport(0.0f, 0.0f, static_cast<Float>(mRmode->fbWidth),
			static_cast<Float>(mRmode->efbHeight), 0.0f, 1.0f);
	}

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
	// Invalidate texture cache in GP
	GXInvalidateTexAll();

	return true;
}

//----------------------------------------------------------------------------
void GXRenderer::EndScene()
{
}

//----------------------------------------------------------------------------
void GXRenderer::DisplayBackBuffer()
{
	// Set Z/Color update to make sure eFB will be cleared at GXCopyDisp.
	// (If you want to control these modes by yourself in your application,
	//  please comment out this part.)
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetColorUpdate(GX_TRUE);

	// Issue display copy command
	GXCopyDisp(mFrameBuffer[mFrameBufferIndex^1], GX_TRUE);

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

	mGXClearColor.r = static_cast<UChar>(rClearColor.R() * 255.0f);
	mGXClearColor.g = static_cast<UChar>(rClearColor.G() * 255.0f);
	mGXClearColor.b = static_cast<UChar>(rClearColor.B() * 255.0f);
	mGXClearColor.a = static_cast<UChar>(rClearColor.A() * 255.0f);
	GXSetCopyClear(mGXClearColor, GX_MAX_Z24);
}

//----------------------------------------------------------------------------
void GXRenderer::DrawElements()
{
	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	// setup the vertex attribute table
	// describes the data
	// args: vat location 0-7, type of data, data format, size, scale
	// so for ex. in the first call we are sending position data with
	// 3 values X,Y,Z of size F32. scale sets the number of fractional
	// bits for non float data.
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);

	GXSetNumChans(1);
	GXSetNumTexGens(0);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

	Matrix34f model;
	mpGeometry->Local.GetTransformation(model);
	// load the modelview matrix into matrix memory
	GXLoadPosMtxImm(mpCamera->GetView() * model, GX_PNMTX0);

	const VertexBuffer* pVBuffer = mpGeometry->VBuffer;

	UChar gxPrimitive = GX_TRIANGLES;
	if (mpGeometry->Type == Geometry::GT_QUADMESH)
	{
		gxPrimitive = GX_QUADS;
	}

	GXBegin(gxPrimitive, GX_VTXFMT0, pVBuffer->GetQuantity());

	for (UInt i = 0; i < pVBuffer->GetQuantity(); i++)
	{
		const Vector3f& rVertex = pVBuffer->Position3(i);
		const Vector3f& rColor = pVBuffer->Color3(i);
		GXPosition3f32(rVertex.X(), rVertex.Y(), rVertex.Z());
		GX_Color3f32(rColor.X(), rColor.Y(), rColor.Z());
	}

	GXEnd();
}