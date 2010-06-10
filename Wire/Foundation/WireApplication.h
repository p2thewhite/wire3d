#pragma once
#ifndef WIREAPPLICATION_H
#define WIREAPPLICATION_H

#include "WireColorRGBA.h"
#include "WireMain.h"
#include "WireSystem.h"

namespace Wire
{

class Renderer;

class Application
{
public:
	// abstract base class
	virtual ~Application();

	virtual Int Main(Int argumentQuantity, Char* arguments[]) = 0;

// 	// Called before the creation and initialization of the renderer.
// 	// Returning 'false' quits the application.
// 	virtual Bool OnPrecreate();
// 
	// Called before main loop.
	// Returning 'false' quits the application.
	virtual Bool OnInitialize() = 0;

	// Called by the main loop
	virtual void OnIdle() = 0;

	// Called before terminating the application.
	virtual void OnTerminate() = 0;

	static void SetApplication(Application* pApplication);
	static Application* GetApplication(); 

	Renderer* GetRenderer() const;

	// Key identifiers. These are platform-specific, so classes that
	// implement the Application interfaces must define these variables.
	// They are not defined by Application.
	UInt KEY_TERMINATE;  // default KEY_ESCAPE, redefine as desired
	static const UInt KEY_ESCAPE;

protected:
	Application(const ColorRGBA backgroundColor, const Char* pWindowTitle,
		Int xPosition, Int yPosition, UInt width, UInt height);

	// the unique application object
	static Application* mpApplication;

	Renderer* mpRenderer;

	ColorRGBA mBackgroundColor;
	const Char* mpWindowTitle;
	Int mXPosition;
	Int mYPosition;
	UInt mWidth;
	UInt mHeight;
};

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
