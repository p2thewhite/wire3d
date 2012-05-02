#include "WireWiiKeyMapper.h"
#include "WireUnmappedPlatformKeyException.h"
#include "WireUnmappedVirtualKeyException.h"
#include "WireSystem.h"
#include <wiiuse/wpad.h>

namespace Wire
{

WiiKeyMapper::WiiKeyMapper()
{
}

WiiKeyMapper::~WiiKeyMapper()
{
}

UInt WiiKeyMapper::GetPlatformKeyForButton(Button button) const
{
	switch (button)
	{
	case BUTTON_A:
		return WPAD_BUTTON_A;
	case BUTTON_B:
		return WPAD_BUTTON_B;
	case BUTTON_1:
		return WPAD_BUTTON_1;
	case BUTTON_2:
		return WPAD_BUTTON_2;
	case BUTTON_HOME:
		return WPAD_BUTTON_HOME;
	case BUTTON_MINUS:
		return WPAD_BUTTON_MINUS;
	case BUTTON_PLUS:
		return WPAD_BUTTON_PLUS;
	case BUTTON_Z:
		return WPAD_NUNCHUK_BUTTON_Z;
	case BUTTON_C:
		return WPAD_NUNCHUK_BUTTON_C;
	default:
		System::Assert("Unknown key.", "WiiPlatformKeyMapper.cpp", 34);
		throw new UnmappedVirtualKeyException(button);
	}
}

UInt WiiKeyMapper::GetPlatformKeyForIRAxis(IRAxis axis) const
{
	return 0;
}

UInt WiiKeyMapper::GetPlatformKeyForDigitalAxis(DigitalAxis axis) const
{
	switch (axis)
	{
	case DIGITAL_LEFT:
		return WPAD_BUTTON_LEFT;
	case DIGITAL_RIGHT:
		return WPAD_BUTTON_RIGHT;
	case DIGITAL_UP:
		return WPAD_BUTTON_UP;
	case DIGITAL_DOWN:
		return WPAD_BUTTON_DOWN;
	default:
		System::Assert("Unknown digital axis.", "WiiPlatformKeyMapper.cpp", 60);
		throw new UnmappedVirtualKeyException(axis);
	}
}

UInt WiiKeyMapper::GetPlatformKeyForAnalogAxis(AnalogAxis axis) const
{
	throw new UnmappedVirtualKeyException(axis);
}

Button WiiKeyMapper::GetButtonForPlatformKey(UInt key) const
{
	switch (key)
	{
	case WPAD_BUTTON_1:
		return BUTTON_1;
	case WPAD_BUTTON_2:
		return BUTTON_2;
	case WPAD_BUTTON_A:
		return BUTTON_A;
	case WPAD_BUTTON_B:
		return BUTTON_B;
	case WPAD_BUTTON_PLUS:
		return BUTTON_PLUS;
	case WPAD_BUTTON_MINUS:
		return BUTTON_MINUS;
	case WPAD_BUTTON_HOME:
		return BUTTON_HOME;
	default:
		System::Assert("Unknown platform key.", "WiiPlatformKeyMapper.cpp", 58);
		// TODO: Invalid key.
		return BUTTON_HOME;
	}
}

IRAxis WiiKeyMapper::GetIRAxisForPlatformKey(UInt key) const
{
	throw new UnmappedPlatformKeyException(key);
}

DigitalAxis WiiKeyMapper::GetDigitalAxisForPlatformKey(UInt key) const
{
	return DIGITAL_LEFT;
}

AnalogAxis WiiKeyMapper::GetAnalogAxisForPlatformKey(UInt key) const
{
	throw new UnmappedPlatformKeyException(key);
}

}
