#pragma once
#ifndef WIREDXAPPLICATION_H
#define WIREDXAPPLICATION_H

#include "../WireApplication.h"

namespace Wire
{

class DXApplication : public Application
{

public:
	DXApplication();
	virtual ~DXApplication();

	virtual Bool OnInitialize();
	virtual void OnTerminate();
	virtual void OnIdle();

private:
	// Entry point to be implemented by the application.  The return value
	// is an exit code, if desired.
	virtual Int Main(Int argumentQuantity, Char* arguments[]);
};

}

#endif
