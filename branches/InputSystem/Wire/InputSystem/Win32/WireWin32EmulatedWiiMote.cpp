#include "WireWin32EmulatedWiiMote.h"
#include "WireMemory.h"
#include "WireWin32KeyMappedDigitalPad.h"
#include "WireWin32KeyMappedButtons.h"
#include "WireWin32KeyMappedIR.h"
#include "WireWin32KeyMappedShake.h"
#include "WireWin32KeyMappedSwing.h"
#include "WireWin32KeyMappedTilt.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32EmulatedWiiMote, MainInputDevice);

Win32EmulatedWiiMote::Win32EmulatedWiiMote()
{
}

Win32EmulatedWiiMote::~Win32EmulatedWiiMote()
{
}

void Win32EmulatedWiiMote::SetUp()
{
	RegisterCapability(WIRE_NEW Win32KeyMappedDigitalPad(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedButtons(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedIR(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedShake(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedSwing(this));
	RegisterCapability(WIRE_NEW Win32KeyMappedTilt(this));
}

}