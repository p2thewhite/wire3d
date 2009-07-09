#pragma once
#ifndef WIREAPPLICATION_H
#define WIREAPPLICATION_H

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
	Application();

	// the unique application object
	static Application* mpApplication;

	Renderer* mpRenderer;
};

}

#endif
