#pragma once
#ifndef WIREDX9APPLICATION_H
#define WIREDX9APPLICATION_H

#include "../WireApplication.h"

namespace Wire
{

class Dx9Application : public Application
{

public:
	Dx9Application();
	virtual ~Dx9Application();

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
