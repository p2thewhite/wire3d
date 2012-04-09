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

const UInt Application::BUTTON_A = 0;
const UInt Application::BUTTON_B = 1;
const UInt Application::BUTTON_LEFT = 2;
const UInt Application::BUTTON_RIGHT = 3;
const UInt Application::BUTTON_UP = 4;
const UInt Application::BUTTON_DOWN = 5;
const UInt Application::BUTTON_1 = 6;
const UInt Application::BUTTON_2 = 7;

const UInt Application::BUTTON_PRESS = 0;
const UInt Application::BUTTON_RELEASE = 1;

//----------------------------------------------------------------------------
Application::Application(const ColorRGBA& rBackgroundColor, const Char*
	pWindowTitle, Int xPosition, Int yPosition, UInt width, UInt height,
	Bool isFullscreen, Bool useVSync)
	:
	mpRenderer(NULL),
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
void Application::OnButton(UInt, UInt)
{
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
