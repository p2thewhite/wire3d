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
		System::Assert("BUTTON_A is not mapped to a key.", "Win32KeyMapper.cpp", 22);
		return 0;
	case BUTTON_B:
		System::Assert("BUTTON_B is not mapped to a key.", "Win32KeyMapper.cpp", 25);
		return 0;
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
		System::Assert("Unknown key.", "Win32KeyMapper.cpp", 42);
		return 0;
	}
}

UInt Win32KeyMapper::GetPlatformKeyForIRAxis(IRAxis axis) const
{
	return 0;
}

UInt Win32KeyMapper::GetPlatformKeyForDigitalAxis(DigitalAxis axis) const
{
	switch (axis)
	{
	case DIGITAL_LEFT:
		return VK_LEFT;
	case DIGITAL_RIGHT:
		return VK_RIGHT;
	case DIGITAL_UP:
		return VK_UP;
	case DIGITAL_DOWN:
		return VK_DOWN;
	default:
		System::Assert("Unknown digital axis.", "Win32KeyMapper.cpp", 63);
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
		System::Assert("Unknown platform key.", "Win32KeyMapper.cpp", 92);
		return INVALID_BUTTON;
	}
}

IRAxis Win32KeyMapper::GetIRAxisForPlatformKey(UInt key) const
{
	return INVALID_IR;
}

DigitalAxis Win32KeyMapper::GetDigitalAxisForPlatformKey(UInt key) const
{
	switch (key)
	{
	case VK_LEFT:
		return DIGITAL_LEFT;
	case VK_RIGHT:
		return DIGITAL_RIGHT;
	case VK_UP:
		return DIGITAL_UP;
	case VK_DOWN:
		return DIGITAL_DOWN;
	default:
		System::Assert("Unknown platform key.", "Win32KeyMapper.cpp", 115);
		return INVALID_DIGITAL;
	}
}

AnalogAxis Win32KeyMapper::GetAnalogAxisForPlatformKey(UInt key) const
{
	return INVALID_ANALOG;
}

}