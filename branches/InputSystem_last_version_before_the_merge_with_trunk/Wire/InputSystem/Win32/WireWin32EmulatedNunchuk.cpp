#include "WireWin32EmulatedNunchuk.h"
#include "WireMemory.h"
#include "WireWin32KeyMappedAnalogPad.h"
#include "WireWin32KeyMappedButtons.h"
#include "WireWin32KeyMappedShake.h"
#include "WireWin32KeyMappedSwing.h"
#include "WireWin32KeyMappedTilt.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32EmulatedNunchuk, InputDeviceExtension);

Win32EmulatedNunchuk::Win32EmulatedNunchuk(const MainInputDevice* pMainInputDevice)
	: InputDeviceExtension(pMainInputDevice)
{
}

Win32EmulatedNunchuk::~Win32EmulatedNunchuk()
{
}

void Win32EmulatedNunchuk::SetUp()
{
	RegisterCapability(WIRE_NEW Win32KeyMappedAnalogPad(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedButtons(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedShake(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedSwing(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedTilt(this));
}

}