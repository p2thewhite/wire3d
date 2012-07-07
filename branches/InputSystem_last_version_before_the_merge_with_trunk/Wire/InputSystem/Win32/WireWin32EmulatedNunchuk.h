#ifndef WIN32EMULATEDNUNCHUK_H_
#define WIN32EMULATEDNUNCHUK_H_

#include "WireInputDeviceExtension.h"
#include "WireRtti.h"

namespace Wire
{

class Win32EmulatedNunchuk : public InputDeviceExtension
{
	WIRE_DECLARE_RTTI;

public:
	Win32EmulatedNunchuk(const MainInputDevice* pMainInputDevice);
	virtual ~Win32EmulatedNunchuk();

	virtual void SetUp();

};

}

#endif // WIN32EMULATEDNUNCHUK_H_
