// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireGXApplication.h"

#include "WireGXRendererInput.h"
#include "WireRenderer.h"

using namespace Wire;

const UInt Application::KEY_ESCAPE = WPAD_BUTTON_HOME;

//----------------------------------------------------------------------------
GXApplication::GXApplication(const ColorRGBA& rBackgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool)
	:
	Application(rBackgroundColor, pWindowTitle, xPosition, yPosition, width,
		height, true)
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
	// allow work to be done before the renderer is created
	if (!s_pApplication->OnPrecreate())
	{
		return -1;
	}

	PdrRendererInput input;
	input.BackgroundColor = mBackgroundColor;
	mpRenderer = WIRE_NEW Renderer(input, 0, 0, mIsFullscreen);
	s_pApplication->KEY_TERMINATE = Application::KEY_ESCAPE;

	// VIInit must be called before PADInit
	PADInit();
	WPAD_ScanPads();
	if (s_pApplication->OnInitialize())
	{
		while (!(WPAD_ButtonsDown(0) & s_pApplication->KEY_TERMINATE))
		{
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
