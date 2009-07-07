#pragma once
#ifndef WIREGXAPPLICATION_H
#define WIREGXAPPLICATION_H

#include "../WireApplication.h"

namespace Wire
{

class GXApplication : public Application
{

public:
	GXApplication();
	virtual ~GXApplication();

	// Entry point to be implemented by the application.  The return value
	// is an exit code, if desired.
	virtual Int Main();

	virtual Bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();

	virtual Int Run(Int argumentQuantity, Char* arguments[]);
};

}

#endif
