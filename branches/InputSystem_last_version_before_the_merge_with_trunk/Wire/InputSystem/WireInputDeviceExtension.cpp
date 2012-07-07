#include "WireInputDeviceExtension.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(InputDeviceExtension, InputDevice);

InputDeviceExtension::InputDeviceExtension(const MainInputDevice* pMainInputDevice)
	: mpMainInputDevice(pMainInputDevice)
{
}

InputDeviceExtension::~InputDeviceExtension()
{
}

const MainInputDevice* InputDeviceExtension::GetMainInputDevice() const
{
	return mpMainInputDevice;
}

}