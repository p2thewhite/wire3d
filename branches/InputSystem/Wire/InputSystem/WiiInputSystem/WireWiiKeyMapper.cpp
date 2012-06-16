#include "WireWiiKeyMapper.h"
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
		System::Assert("Unknown button.", "WiiKeyMapper.cpp", 39);
		return WPAD_INVALID_BUTTON;
	}
}

UInt WiiKeyMapper::GetPlatformKeyForIRAxis(IRAxis axis) const
{
	return 0;
}

UInt WiiKeyMapper::GetPlatformKeyForIRAxisRotation(EulerAngle eulerAngle) const
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
		System::Assert("Unknown digital axis.", "WiiKeyMapper.cpp", 62);
		return WPAD_INVALID_BUTTON;
	}
}

UInt WiiKeyMapper::GetPlatformKeyForAnalogAxis(AnalogAxis axis) const
{
	return 0;
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
	case WPAD_BUTTON_MINUS:
		return BUTTON_MINUS;
	case WPAD_BUTTON_PLUS:
		return BUTTON_PLUS;
	case WPAD_BUTTON_HOME:
		return BUTTON_HOME;
	default:
		System::Assert("Unknown platform key.", "WiiKeyMapper.cpp", 91);
		return INVALID_BUTTON;
	}
}

IRAxis WiiKeyMapper::GetIRAxisForPlatformKey(UInt key) const
{
	return INVALID_IR;
}

EulerAngle WiiKeyMapper::GetIRAxisRotationForPlatformKey(UInt key) const
{
	return INVALID_EULER_ANGLE;
}

DigitalAxis WiiKeyMapper::GetDigitalAxisForPlatformKey(UInt key) const
{
	switch (key)
	{
	case WPAD_BUTTON_LEFT:
		return DIGITAL_LEFT;
	case WPAD_BUTTON_RIGHT:
		return DIGITAL_RIGHT;
	case WPAD_BUTTON_UP:
		return DIGITAL_UP;
	case WPAD_BUTTON_DOWN:
		return DIGITAL_DOWN;
	default:
		System::Assert("Unknown platform key.", "WiiKeyMapper.cpp", 114);
		return INVALID_DIGITAL;
	}
}

AnalogAxis WiiKeyMapper::GetAnalogAxisForPlatformKey(UInt key) const
{
	return INVALID_ANALOG;
}

}
