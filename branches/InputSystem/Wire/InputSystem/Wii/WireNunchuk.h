#ifndef NUNCHUK_H_
#define NUNCHUK_H_

#include "WireInputDeviceExtension.h"
#include "WireMainInputDevice.h"
#include "WireRtti.h"

namespace Wire
{

class Nunchuk : public InputDeviceExtension
{
	WIRE_DECLARE_RTTI;

public:
	Nunchuk(const MainInputDevice* pMainInputDevice);
	virtual ~Nunchuk();

	virtual void SetUp();

};

}

#endif // NUNCHUK_H_
