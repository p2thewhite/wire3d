#include "DemoInit.h"

//-------------------------------------------------------------------------
DemoInit::DemoInit()
{
	mFrameBuffer[0] = NULL;
	mFrameBuffer[1] = NULL;

	mFrameBufferIndex = 0;
}

//-------------------------------------------------------------------------
DemoInit::~DemoInit()
{

}

//-------------------------------------------------------------------------
void DemoInit::StartVI()
{
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

//-------------------------------------------------------------------------
void DemoInit::InitGX()
{
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
}

//-------------------------------------------------------------------------
void DemoInit::ConfigureMem()
{
	mDemoFifoBuffer = memalign(32, DEFAULT_FIFO_SIZE);
	memset(mDemoFifoBuffer,0 , DEFAULT_FIFO_SIZE);
}

//-------------------------------------------------------------------------
void DemoInit::Init()
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

	ConfigureMem();

	GXInit(mDemoFifoBuffer, DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	GXColor background = {0, 0, 0, 0xff};
	GXSetCopyClear(background, GX_MAX_Z24);

	InitGX();

	StartVI();
}

//-------------------------------------------------------------------------
void DemoInit::BeforeRender()
{
	// Set up viewport (This is inappropriate for full-frame AA.)
	if (mRmode->field_rendering)
	{
		GXSetViewportJitter(0.0f, 0.0f, static_cast<float>(mRmode->fbWidth),
			static_cast<float>(mRmode->efbHeight), 0.0f, 1.0f,
			VIGetNextField());
	}
	else
	{
		GXSetViewport(0.0f, 0.0f, static_cast<float>(mRmode->fbWidth),
			static_cast<float>(mRmode->efbHeight), 0.0f, 1.0f);
	}

	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
	// Invalidate texture cache in GP
	GXInvalidateTexAll();
}

//-------------------------------------------------------------------------
void DemoInit::DoneRender()
{
	// Set Z/Color update to make sure eFB will be cleared at GXCopyDisp.
	// (If you want to control these modes by yourself in your application,
	//  please comment out this part.)
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetColorUpdate(GX_TRUE);

	// Issue display copy command
	GXCopyDisp(mFrameBuffer[mFrameBufferIndex^1], GX_TRUE);

	// Wait until everything is drawn and copied into XFB.
	GXDrawDone();

	// Set the next frame buffer
	SwapBuffers();
}

//-------------------------------------------------------------------------
void DemoInit::SwapBuffers()
{
	mFrameBufferIndex ^= 1;		// flip framebuffer

	// Display the buffer which was just filled by GXCopyDisplay
	VISetNextFrameBuffer(mFrameBuffer[mFrameBufferIndex]);

	// Tell VI device driver to write the current VI settings so far
	VIFlush();

	VIWaitForRetrace();
}
