#include "WireApplication.h"
#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include <stdlib.h>

//----------------------------------------------------------------------------
Double System::GetTime()
{
	static ULongLong msInitialTicks = 0;

	if (!msInitializedTime)
	{
		msInitializedTime = true;
		msInitialTicks = gettime();
	}

	ULongLong ticks = gettime();
	ULongLong deltaTicks = ticks - msInitialTicks;

	Double deltaSec = static_cast<Double>(ticks_to_millisecs(deltaTicks));

	return 0.001 * deltaSec;
}

//----------------------------------------------------------------------------
void System::Print(const Char* pFormat, ...)
{
	static Bool sIsInitialized = false;

	if (!sIsInitialized)
	{
		Application* pApp = Application::GetApplication();
		Renderer* pRenderer = pApp->GetRenderer();
		PdrRendererData* pData = pRenderer->GetRendererData();
		void* pFrameBuffer = pData->GetFramebuffer();
		GXRenderModeObj* pRMode = pData->GetRenderMode();

		// Initialize the console, required for printf
		console_init(pFrameBuffer, 20, 20, pRMode->fbWidth, pRMode->xfbHeight,
			pRMode->fbWidth * VI_DISPLAY_PIX_SZ);
		sIsInitialized = true;
	}

	va_list args;
	va_start(args, pFormat);

	vprintf(pFormat, args);

	va_end(args);
}

//----------------------------------------------------------------------------
void System::Assert(const Char* pExpression, const Char* pFile,
	Int lineNumber)
{
	Application* pApp = Application::GetApplication();
	Renderer* pRenderer = pApp->GetRenderer();
	PdrRendererData* pData = pRenderer->GetRendererData();
	pData->SetFramebufferIndex(1);
	pRenderer->DisplayBackBuffer();

	Print("\x1b[4;0H");
	Print("Assertion failed, %s, %s, line %d\n\n", pExpression, pFile,
		lineNumber);

	Print("Press 'HOME' button to exit.");

	do
	{
		WPAD_ScanPads();
	} while (!(WPAD_ButtonsDown(0) & Application::KEY_ESCAPE));

	exit(0);
}
