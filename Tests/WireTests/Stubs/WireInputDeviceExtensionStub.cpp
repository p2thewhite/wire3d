#include "WireInputDeviceExtensionStub.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, InputDeviceExtensionStub, InputDeviceExtension);

InputDeviceExtensionStub::InputDeviceExtensionStub(MainInputDevice* pMainInputDevice)
	: InputDeviceExtension(pMainInputDevice)
{
}

InputDeviceExtensionStub::~InputDeviceExtensionStub()
{
}

void InputDeviceExtensionStub::SetUp()
{
	RegisterCapability(new InputCapabilityStub(this));
}