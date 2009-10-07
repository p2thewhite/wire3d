#pragma once
#ifndef WIREAPPLICATION_H
#define WIREAPPLICATION_H

#include "WireColorRGBA.h"
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
	virtual void OnIdle() = 0;

	// Called before terminating the application.
	virtual void OnTerminate() = 0;

	static void SetApplication(Application* pApplication);
	static Application* GetApplication();

protected:
	Application(const ColorRGBA backgroundColor, const Char* pWindowTitle,
		Int xPosition, Int yPosition, Int width, Int height);

	// the unique application object
	static Application* mpApplication;

	Renderer* mpRenderer;

	ColorRGBA mBackgroundColor;
	const Char* mpWindowTitle;
	Int mXPosition;
	Int mYPosition;
	Int mWidth;
	Int mHeight;
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
