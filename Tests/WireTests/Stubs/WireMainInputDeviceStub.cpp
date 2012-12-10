#include "WireMainInputDeviceStub.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, MainInputDeviceStub, MainInputDevice);

MainInputDeviceStub::MainInputDeviceStub() 
	: MainInputDevice()
{
}

MainInputDeviceStub::~MainInputDeviceStub()
{
}

void MainInputDeviceStub::SetUp()
{
	RegisterCapability(new InputCapabilityStub(this));
}