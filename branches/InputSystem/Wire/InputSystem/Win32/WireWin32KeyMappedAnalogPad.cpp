#include "WireWin32KeyMappedAnalogPad.h"
#include "WireWin32InputDataBuffer.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedAnalogPad, AnalogPad);

Win32KeyMappedAnalogPad::Win32KeyMappedAnalogPad(const InputDevice* pParent)
	: AnalogPad(pParent)
{
}

Win32KeyMappedAnalogPad::~Win32KeyMappedAnalogPad()
{
}

Float Win32KeyMappedAnalogPad::GetDown() const
{
	return 0;
}

Float Win32KeyMappedAnalogPad::GetLeft() const
{
	return 0;
}

Float Win32KeyMappedAnalogPad::GetRight() const
{
	return 0;
}

Float Win32KeyMappedAnalogPad::GetUp() const
{
	return 0;
}

}