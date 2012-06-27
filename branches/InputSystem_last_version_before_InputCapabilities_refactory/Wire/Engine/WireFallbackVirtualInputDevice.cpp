#include "WireFallbackVirtualInputDevice.h"

namespace Wire
{

FallbackVirtualInputDevice::FallbackVirtualInputDevice()
{
}

FallbackVirtualInputDevice::~FallbackVirtualInputDevice()
{
}

void FallbackVirtualInputDevice::Capture()
{
	// Do nothing.
}

const char* FallbackVirtualInputDevice::GetName() const
{
	return "Fallback[There's no input device]";
}

Bool FallbackVirtualInputDevice::GetButton(Button button) const
{
	return false;
}

Float FallbackVirtualInputDevice::GetIRAxis(IRAxis axis) const
{
	return 0;
}

Float FallbackVirtualInputDevice::GetIRAxisRotation(EulerAngle eulerAngle) const
{
	return 0;
}

Float FallbackVirtualInputDevice::GetDigitalAxis(DigitalAxis axis) const
{
	return 0;
}

Float FallbackVirtualInputDevice::GetAnalogAxis(AnalogAxis axis) const
{
	return 0;
}

Bool FallbackVirtualInputDevice::HasCapability(InputCapability capability) const
{
	return false;
}

const set<InputCapability>& FallbackVirtualInputDevice::GetCapabilities() const
{
	return mCapabilities;
}

}
