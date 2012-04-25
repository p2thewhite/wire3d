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
#include <fat.h>

using namespace Wire;

const UInt Application::KEY_ESCAPE = WPAD_BUTTON_HOME;

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
	s_pApplication->KEY_TERMINATE = Application::KEY_ESCAPE;

	// VIInit must be called before PADInit
	PAD_Init();
	WPAD_Init();
	WPAD_ScanPads();
	if (s_pApplication->OnInitialize())
	{
		while (!(WPAD_ButtonsDown(0) & s_pApplication->KEY_TERMINATE))
		{
			HandleButtons();
			s_pApplication->OnIdle();
			WPAD_ScanPads();
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

//----------------------------------------------------------------------------
void GXApplication::HandleButtons()
{
	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A)
	{
		s_pApplication->OnButton(BUTTON_A, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_A)
	{
		s_pApplication->OnButton(BUTTON_A, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_B)
	{
		s_pApplication->OnButton(BUTTON_B, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_B)
	{
		s_pApplication->OnButton(BUTTON_B, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_LEFT)
	{
		s_pApplication->OnButton(BUTTON_LEFT, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_LEFT)
	{
		s_pApplication->OnButton(BUTTON_LEFT, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_RIGHT)
	{
		s_pApplication->OnButton(BUTTON_RIGHT, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_RIGHT)
	{
		s_pApplication->OnButton(BUTTON_RIGHT, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_UP)
	{
		s_pApplication->OnButton(BUTTON_UP, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_UP)
	{
		s_pApplication->OnButton(BUTTON_UP, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_DOWN)
	{
		s_pApplication->OnButton(BUTTON_DOWN, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_DOWN)
	{
		s_pApplication->OnButton(BUTTON_DOWN, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_1)
	{
		s_pApplication->OnButton(BUTTON_1, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_1)
	{
		s_pApplication->OnButton(BUTTON_DOWN, BUTTON_RELEASE);
	}

	if (WPAD_ButtonsDown(0) & WPAD_BUTTON_2)
	{
		s_pApplication->OnButton(BUTTON_2, BUTTON_PRESS);
	}

	if (WPAD_ButtonsUp(0) & WPAD_BUTTON_2)
	{
		s_pApplication->OnButton(BUTTON_2, BUTTON_RELEASE);
	}
}
