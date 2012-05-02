#include "WireFallbackVirtualInputDevice.h"
#include <set>

using namespace std;

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
	return false;
}

Float FallbackVirtualInputDevice::GetDigitalAxis(DigitalAxis axis) const
{
	return false;
}

Float FallbackVirtualInputDevice::GetAnalogAxis(AnalogAxis axis) const
{
	return false;
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
