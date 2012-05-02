// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXApplication.h"

#include "WireGXRendererInput.h"
#include "WireRenderer.h"
#include "WireGXRendererData.h"
#include "WireWiiInputSystem.h"
#include <fat.h>

using namespace Wire;

//----------------------------------------------------------------------------
GXApplication::GXApplication(const ColorRGBA& rBackgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool, Bool useVSync)
	:
	Application(rBackgroundColor, pWindowTitle, xPosition, yPosition, width,
		height, true, useVSync)
{
}

//----------------------------------------------------------------------------
GXApplication::~GXApplication()
{
	WIRE_DELETE mpRenderer;
	mpRenderer = NULL;
	WIRE_DELETE mpInputSystem;
	mpInputSystem = NULL;
}

//----------------------------------------------------------------------------
Int GXApplication::Main(Int argumentQuantity, Char* arguments[])
{
	fatInitDefault();

	// allow work to be done before the renderer is created
	if (!s_pApplication->OnPrecreate())
	{
		return -1;
	}

	PdrRendererInput input;
	input.BackgroundColor = mBackgroundColor;
	mpRenderer = WIRE_NEW Renderer(input, 0, 0, mIsFullscreen, mUseVSync);

	PdrRendererData* pRendererData = mpRenderer->GetRendererData();
	GXRenderModeObj* pRMode = pRendererData->RMode;

	mpInputSystem = WIRE_NEW WiiInputSystem(pRMode->fbWidth, pRMode->xfbHeight);

	if (s_pApplication->OnInitialize())
	{
		mpInputSystem->DiscoverInputDevices();
		mpInputSystem->Capture();

		while (!mpInputSystem->GetInputDevice(0)->GetButton(BUTTON_HOME))
		{
			s_pApplication->OnIdle();
			mpInputSystem->Capture();

			// Try to discover new input devices.
			mpInputSystem->DiscoverInputDevices();
		}
	}

	s_pApplication->OnTerminate();

	return 0;
}

//----------------------------------------------------------------------------
Bool GXApplication::OnPrecreate()
{
	return true;
}

//----------------------------------------------------------------------------
Bool GXApplication::OnInitialize()
{
	return true;
}

//----------------------------------------------------------------------------
void GXApplication::OnTerminate()
{
}

//----------------------------------------------------------------------------
void GXApplication::OnIdle()
{
}
