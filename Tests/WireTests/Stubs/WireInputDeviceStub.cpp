#include "WireInputDeviceStub.h"

#include "WireInputCapabilityStub.h"

using namespace Wire;

InputDeviceStub::InputDeviceStub()
	: InputDevice()
{
}

InputDeviceStub::~InputDeviceStub()
{
}

void InputDeviceStub::SetUp()
{
	RegisterCapability(new InputCapabilityStub(this));
}