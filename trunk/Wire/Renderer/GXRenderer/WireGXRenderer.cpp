#include "WireGXRenderer.h"
#include "../Foundation/WireMatrix4.h"
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
	//  please comment out this part.)
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

	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

	GXSetCullMode(GX_CULL_FRONT);

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

	Matrix34F model;
	mpGeometry->World.GetTransformation(model);
	// load the modelview matrix into matrix memory
	Matrix34F view = mViewMatrix;
	view[2][2] = -view[2][2];

	GXLoadPosMtxImm(view * model, GX_PNMTX0);

	const VertexBuffer* pVBuffer = mpGeometry->VBuffer;
	const IndexBuffer* pIBuffer = mpGeometry->IBuffer;

	GXBegin(GX_TRIANGLES, GX_VTXFMT0, pIBuffer->GetIndexQuantity());

	for (UInt i = 0; i < pIBuffer->GetIndexQuantity(); i++)
	{
		UInt index = (*pIBuffer)[i];
		const Vector3F& rVertex = pVBuffer->Position3(index);
		const ColorRGB& rColor = pVBuffer->Color3(index);
		GXPosition3f32(rVertex.X(), rVertex.Y(), rVertex.Z());
		GX_Color3f32(rColor.R(), rColor.G(), rColor.B());
	}

	GXEnd();
}
