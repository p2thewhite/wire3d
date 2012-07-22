// Wire3D by Roman Rath (antibyte@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireApplication.h"
#include "WireMain.h"

using namespace Wire;

Application* Application::s_pApplication = NULL;

//----------------------------------------------------------------------------
Application::Application(const ColorRGBA& rBackgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool isFullscreen, Bool useVSync)
	:
	mpRenderer(NULL),
	mpInputSystem(NULL),
	mBackgroundColor(rBackgroundColor),
	mpWindowTitle(pWindowTitle),
	mXPosition(xPosition),
	mYPosition(yPosition),
	mWidth(width),
	mHeight(height),
	mIsFullscreen(isFullscreen),
	mUseVSync(useVSync)
{
}

//----------------------------------------------------------------------------
Application::~Application()
{
}

//----------------------------------------------------------------------------
void Application::SetApplication(Application* pApplication)
{
	s_pApplication = pApplication;
}

//----------------------------------------------------------------------------
Application* Application::GetApplication()
{
	return s_pApplication;
}

//----------------------------------------------------------------------------
void Application::OnResize(UInt width, UInt height)
{
	if (mpRenderer)
	{
		mpRenderer->Resize(width, height);
		mWidth = width;
		mHeight = height;
	}
}
