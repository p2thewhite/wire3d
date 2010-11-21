#pragma once
#ifndef WIREGXAPPLICATION_H
#define WIREGXAPPLICATION_H

#include "WireApplication.h"
#include "WireColorRGBA.h"

namespace Wire
{

class GXApplication : public Application
{

public:
	GXApplication(const ColorRGBA backgroundColor = ColorRGBA::BLACK,
		const Char* pWindowTitle = NULL, Int xPosition = 0, Int yPosition = 0,
		Int width = 640, Int height = 480);

	virtual ~GXApplication();

	virtual Bool OnPrecreate();
	virtual Bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();

private:
	// Entry point to be implemented by the application. The return value is
	// an exit code, if desired.
	virtual Int Main(Int argumentQuantity, Char* arguments[]);
};

}

#endif
