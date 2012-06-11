#include "WireInputDevice.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI(Wire, InputDevice, Object);

InputDevice::InputDevice(const PlatformKeyMapper* pPlatformKeyMapper) :
		mpPlatformKeyMapper(pPlatformKeyMapper)
{
}

InputDevice::~InputDevice()
{
}

}
