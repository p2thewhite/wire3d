#ifndef INPUTDEVICEEXTENSION_H_
#define INPUTDEVICEEXTENSION_H_

#include "WireInputDevice.h"
#include "WireRtti.h"
#include "WireMainInputDevice.h"

namespace Wire
{

class InputDeviceExtension : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	InputDeviceExtension(const MainInputDevice* pMainInputDevice);
	virtual ~InputDeviceExtension();

protected:
	const MainInputDevice* GetMainInputDevice() const;
private:
	const MainInputDevice* mpMainInputDevice;

};

}

#endif // INPUTDEVICEEXTENSION_H_
