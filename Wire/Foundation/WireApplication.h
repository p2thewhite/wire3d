#pragma once
#ifndef WIREAPPLICATION_H
#define WIREAPPLICATION_H

#include "WireTypes.h"

namespace Wire
{

class Renderer;

class Application
{
public:
	// abstract base class
	virtual ~Application();

	// Called before the creation and initialization of the renderer.
	// Returning 'false' quits the application.
	virtual Bool OnPrecreate() = 0;

	// Called before main loop.
	// Returning 'false' quits the application.
	virtual Bool OnInitialize() = 0;

	// Called each render frame.
	virtual void OnDisplay() = 0;

	// Called before terminating the application.
	virtual void OnTerminate() = 0;

protected:
	Application();

//	virtual Int Run(Int argumentQuantity, Char* arguments[]) = 0;

	Renderer* mpRenderer;
};

}

#endif /* WIREAPPLICATION_H */
