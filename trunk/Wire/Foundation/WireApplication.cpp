// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireApplication.h"
#include "WireMain.h"

using namespace Wire;

Application* Application::smpApplication = NULL;

//----------------------------------------------------------------------------
Application::Application(const ColorRGBA backgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool isFullscreen)
	:
	mpRenderer(NULL),
	mBackgroundColor(backgroundColor),
	mpWindowTitle(pWindowTitle),
	mXPosition(xPosition),
	mYPosition(yPosition),
	mWidth(width),
	mHeight(height),
	mIsFullscreen(isFullscreen)
{
}

//----------------------------------------------------------------------------
Application::~Application()
{
}

//----------------------------------------------------------------------------
void Application::SetApplication(Application* pApplication)
{
	smpApplication = pApplication;
}

//----------------------------------------------------------------------------
Application* Application::GetApplication()
{
	return smpApplication;
}

//----------------------------------------------------------------------------
void Application::OnResize(UInt width, UInt height)
{
	if (mpRenderer)
	{
		mpRenderer->Resize(width, height);
	}

	mWidth = width;
	mHeight = height;
}
