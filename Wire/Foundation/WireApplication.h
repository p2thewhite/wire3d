// Wire3D by antibyte@gmail.com
// http://wire3d.googlecode.com
// Copyright(c) 2009-2011. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREAPPLICATION_H
#define WIREAPPLICATION_H

#include "WireEngine.h"
#include "WireFoundation.h"

namespace Wire
{

class Renderer;

class Application
{
public:
	// abstract base class
	virtual ~Application();

	virtual Int Main(Int argumentQuantity, Char* arguments[]) = 0;

 	// Called before the creation and initialization of the renderer.
 	// Returning 'false' quits the application.
 	virtual Bool OnPrecreate() = 0;

	// Called before main loop. Returning 'false' quits the application.
	virtual Bool OnInitialize() = 0;

	// Called by the main loop
	virtual void OnIdle() = 0;

	// Called before terminating the application.
	virtual void OnTerminate() = 0;

	virtual void OnResize(UInt width, UInt height);

	static void SetApplication(Application* pApplication);
	static Application* GetApplication(); 

	inline Renderer* GetRenderer() const;

	// Key identifiers. These are platform-specific, so classes that
	// implement the Application interface must define these variables.
	// They are not defined by Application.
	UInt KEY_TERMINATE;  // default KEY_ESCAPE, redefine as desired
	static const UInt KEY_ESCAPE;

protected:
	Application(const ColorRGBA& rBackgroundColor, const Char* pWindowTitle,
		Int xPosition, Int yPosition, UInt width, UInt height,
		Bool isFullscreen, Bool useVSync);

	// the unique application object
	static Application* s_pApplication;

	Renderer* mpRenderer;

	ColorRGBA mBackgroundColor;
	const Char* mpWindowTitle;
	Int mXPosition;
	Int mYPosition;
	UInt mWidth;
	UInt mHeight;
	Bool mIsFullscreen;
	Bool mUseVSync;
};

#include "WireApplication.inl"

}

//----------------------------------------------------------------------------
#ifdef WIRE_WIN
#include "Dx9/WireDx9Application.h"
#define WIREAPPLICATION Dx9Application
#else
#include "Wii/WireGXApplication.h"
#define WIREAPPLICATION GXApplication
#endif

#endif
