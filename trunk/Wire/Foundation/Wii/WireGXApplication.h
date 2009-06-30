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
	virtual int Main();

	virtual Bool OnInitialize();
};

}

#endif /* WIREGXAPPLICATION_H */
