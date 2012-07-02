#ifndef WIN32EMULATEDWIIMOTE_H_
#define WIN32EMULATEDWIIMOTE_H_

#include "WireMainInputDevice.h"
#include "WireRtti.h"

namespace Wire
{

class Win32EmulatedWiiMote : public MainInputDevice
{
	WIRE_DECLARE_RTTI;

public:
	Win32EmulatedWiiMote();
	virtual ~Win32EmulatedWiiMote();

	virtual void SetUp();

};

}

#endif // WIN32EMULATEDWIIMOTE_H_
