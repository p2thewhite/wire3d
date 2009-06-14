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

	// abstract base class
	virtual ~GXApplication();

	virtual Bool OnPrecreate();
	virtual Bool OnInitialize();
	virtual void OnDisplay();
	virtual void OnTerminate();

protected:

};

}

#endif /* WIREGXAPPLICATION_H */
