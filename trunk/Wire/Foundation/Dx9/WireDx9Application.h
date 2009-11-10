#pragma once
#ifndef WIREDX9APPLICATION_H
#define WIREDX9APPLICATION_H

#include "../WireApplication.h"
#include "../WireColorRGBA.h"

namespace Wire
{

class Dx9Application : public Application
{

public:
	Dx9Application(const ColorRGBA backgroundColor = ColorRGBA::BLACK,
		const Char* windowTitle = NULL, Int xPosition = 0, Int yPosition = 0,
		Int width = 640, Int height = 480);
	virtual ~Dx9Application();

	virtual Bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();

	Int GetWindowID() const;

private:
	// Entry point to be implemented by the application.  The return value
	// is an exit code, if desired.
	virtual Int Main(Int argumentQuantity, Char* arguments[]);

	// An identifier for the window (representation is platform-specific).
	Int mWindowID;
};

}

#endif
