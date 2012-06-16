#include "WireWin32KeyMapper.h"
#include "WireSystem.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire 
{

Win32KeyMapper::Win32KeyMapper(void)
{
}

Win32KeyMapper::~Win32KeyMapper(void)
{
}

UInt Win32KeyMapper::GetPlatformKeyForButton(Button button) const 
{
	switch (button)
	{
	case BUTTON_A:
		return VK_RBUTTON;
	case BUTTON_B:
		return VK_LBUTTON;
	case BUTTON_1:
		return 0x31;
	case BUTTON_2:
		return 0x32;
	case BUTTON_HOME:
		return VK_ESCAPE;
	case BUTTON_MINUS:
		return VK_OEM_MINUS;
	case BUTTON_PLUS:
		return VK_OEM_PLUS;
	case BUTTON_Z:
		return VK_PRIOR;
	case BUTTON_C:
		return VK_NEXT;
	default:
		System::Assert("Unknown button.", "Win32KeyMapper.cpp", 40);
		return 0;
	}
}

UInt Win32KeyMapper::GetPlatformKeyForIRAxis(IRAxis axis) const
{
	return 0;
}

UInt Win32KeyMapper::GetPlatformKeyForIRAxisRotation(EulerAngle eulerAngle) const
{
	switch (eulerAngle)
	{
	case YAW:
		return VK_LEFT;
	case PITCH:
		return VK_RIGHT;
	case ROLL:
		return VK_UP;
	default:
		System::Assert("Unknown euler angle.", "Win32KeyMapper.cpp", 61);
		return 0;
	}
}

UInt Win32KeyMapper::GetPlatformKeyForDigitalAxis(DigitalAxis axis) const
{
	switch (axis)
	{
	case DIGITAL_LEFT:
		return 0x41;
	case DIGITAL_RIGHT:
		return 0x44;
	case DIGITAL_UP:
		return 0x57;
	case DIGITAL_DOWN:
		return 0x53;
	default:
		System::Assert("Unknown digital axis.", "Win32KeyMapper.cpp", 79);
		return 0;
	}
}

UInt Win32KeyMapper::GetPlatformKeyForAnalogAxis(AnalogAxis axis) const
{
	return 0;
}

Button Win32KeyMapper::GetButtonForPlatformKey(UInt key) const
{
	switch (key)
	{
	case VK_RBUTTON:
		return BUTTON_A;
	case VK_LBUTTON:
		return BUTTON_B;
	case 0x31:
		return BUTTON_1;
	case 0x32:
		return BUTTON_2;
	case VK_OEM_MINUS:
		return BUTTON_MINUS;
	case VK_OEM_PLUS:
		return BUTTON_PLUS;
	case VK_RETURN:
		return BUTTON_HOME;
	default:
		System::Assert("Unknown platform key.", "Win32KeyMapper.cpp", 108);
		return INVALID_BUTTON;
	}
}

IRAxis Win32KeyMapper::GetIRAxisForPlatformKey(UInt key) const
{
	return INVALID_IR;
}

EulerAngle Win32KeyMapper::GetIRAxisRotationForPlatformKey(UInt key) const
{
	switch (key)
	{
	case VK_LEFT:
		return YAW;
	case VK_RIGHT:
		return PITCH;
	case VK_UP:
		return ROLL;
	default:
		System::Assert("Unknown platform key.", "Win32KeyMapper.cpp", 129);
		return INVALID_EULER_ANGLE;
	}
}

DigitalAxis Win32KeyMapper::GetDigitalAxisForPlatformKey(UInt key) const
{
	switch (key)
	{
	case 0x41:
		return DIGITAL_LEFT;
	case 0x44:
		return DIGITAL_RIGHT;
	case 0x57:
		return DIGITAL_UP;
	case 0x53:
		return DIGITAL_DOWN;
	default:
		System::Assert("Unknown platform key.", "Win32KeyMapper.cpp", 147);
		return INVALID_DIGITAL;
	}
}

AnalogAxis Win32KeyMapper::GetAnalogAxisForPlatformKey(UInt key) const
{
	return INVALID_ANALOG;
}

}