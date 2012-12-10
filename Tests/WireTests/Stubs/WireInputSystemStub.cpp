#include "WireInputSystemStub.h"

using namespace Wire;

InputSystemStub::InputSystemStub()
	: InputSystem()
{
}

InputSystemStub::InputSystemStub(const TArray<MainInputDevicePtr>& rInitialInputDevices)
	: InputSystem()
{
	mInitialInputDevices = rInitialInputDevices;
}

InputSystemStub::~InputSystemStub()
{
}

void InputSystemStub::Capture()
{
}

Bool InputSystemStub::DiscoverDevices()
{
	for (UInt i = 0; i < mInitialInputDevices.GetQuantity(); i++)
	{
		AddDevice(mInitialInputDevices[i]);
	}

	return true;
}