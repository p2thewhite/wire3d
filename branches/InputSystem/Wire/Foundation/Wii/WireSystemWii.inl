// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireApplication.h"
#include "WireGXRendererData.h"
#include "WireRenderer.h"
#include "WireInputSystem.h"
#include "WireButtons.h"
#include <stdlib.h>
#include <cstring>

//----------------------------------------------------------------------------
Double System::GetTime()
{
	static ULongLong s_InitialTicks = 0;

	if (!s_InitializedTime)
	{
		s_InitializedTime = true;
		s_InitialTicks = gettime();
	}

	ULongLong ticks = gettime();
	ULongLong deltaTicks = ticks - s_InitialTicks;

	Double deltaSec = static_cast<Double>(ticks_to_millisecs(deltaTicks));

	return 0.001 * deltaSec;
}

//----------------------------------------------------------------------------
void System::Print(const Char* pFormat, ...)
{
	static Bool s_IsInitialized = false;

	if (!s_IsInitialized)
	{
		Application* pApp = Application::GetApplication();
		Renderer* pRenderer = pApp->GetRenderer();
		PdrRendererData* pData = pRenderer->GetRendererData();
		void* pFrameBuffer = pData->GetFramebuffer();
		GXRenderModeObj* pRMode = pData->RMode;

		// Initialize the console, required for printf
		console_init(pFrameBuffer, 20, 20, pRMode->fbWidth, pRMode->xfbHeight,
			pRMode->fbWidth * VI_DISPLAY_PIX_SZ);
		s_IsInitialized = true;
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

	InputSystem* pInputSystem = pApp->GetInputSystem();
	if (pInputSystem->GetMainInputDevicesCount() > 0 
		&& pInputSystem->GetMainInputDevice(0)->HasCapability(Buttons::TYPE))
	{
		Print("Press 'A' to ignore and continue, or 'HOME' button to exit.");
	
		Buttons* pButtons = static_cast<Buttons*>(pInputSystem->GetMainInputDevice(0)->GetCapability(Buttons::TYPE));
		do
		{
			pInputSystem->Capture();
			if (pButtons->GetButton(BUTTON_HOME))
			{
				exit(0);
			}
	
		} while (!(pButtons->GetButton(BUTTON_A)));
	}
	else
	{
		exit(0);
	}
}

//----------------------------------------------------------------------------
FILE* System::Fopen(const Char* pFilename, const Char* pMode)
{
	return fopen(pFilename, pMode);
}

//----------------------------------------------------------------------------
Char* System::Strcpy(Char* pDst, size_t dstSize, const Char* pSrc)
{
	if (!pDst || dstSize == 0 || !pSrc)
	{
		// Be consistent with the behavior of strcpy_s.
		return NULL;
	}

	size_t srcLen = strlen(pSrc);
	if ((srcLen + 1) > dstSize)
	{
		// The source string is too large to copy to the destination.  To
		// be consistent with strcpy_s, return null as an indication that the
		// copy failed.
		return NULL;
	}

	strncpy(pDst, pSrc, srcLen);
	pDst[srcLen] = 0;
	return pDst;
}

//----------------------------------------------------------------------------
Char* System::Strcat(Char* pDst, size_t dstSize, const Char* pSrc)
{
	if (!pDst || dstSize == 0 || !pSrc)
	{
		// Be consistent with the behavior of strcat_s.
		return NULL;
	}

	size_t srcLen = strlen(pSrc);
	size_t dstLen = strlen(pDst);
	size_t sumLen = srcLen + dstLen;
	if ((sumLen + 1) > dstSize)
	{
		// The source string is too large to append to the destination.  To
		// be consistent with strcat_s, return null as an indication that
		// the concatenation failed.
		return 0;
	}

	strncat(pDst, pSrc, srcLen);
	pDst[sumLen] = 0;
	return pDst;
}

//----------------------------------------------------------------------------
Char* System::Strncpy(Char* pDst, size_t dstSize, const Char* pSrc,
	size_t srcSize)
{
	if (!pDst || dstSize == 0 || !pSrc || srcSize == 0)
	{
		// Be consistent with the behavior of strncpy_s.
		return NULL;
	}

	if ((srcSize + 1) > dstSize)
	{
		// The source string is too large to copy to the destination.  To
		// be consistent with strncpy_s, return null as an indication that
		// the copy failed.
		return NULL;
	}

	strncpy(pDst, pSrc, srcSize);
	return pDst;
}

//----------------------------------------------------------------------------
Int System::Sprintf(Char* pDst, size_t dstSize, const Char* pFormat, ...)
{
	if (!pDst || dstSize == 0 || !pFormat)
	{
		return -1;
	}

	va_list args;
	va_start(args, pFormat);

	Int numWritten = vsprintf(pDst, pFormat, args);

	va_end(args);
	return numWritten;
}

//----------------------------------------------------------------------------
System::Platform System::GetPlatform()
{
	return System::PF_WII;
}
