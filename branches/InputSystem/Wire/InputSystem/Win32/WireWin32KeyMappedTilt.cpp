#include "WireWin32KeyMappedTilt.h"
#include "WireWin32InputDataBuffer.h"
#include "WireButton.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedTilt, Tilt);

Win32KeyMappedTilt::Win32KeyMappedTilt(const InputDevice* pParent)
	: Tilt(pParent)
{
}

Win32KeyMappedTilt::~Win32KeyMappedTilt()
{
}

Float Win32KeyMappedTilt::GetBackward() const
{
	return 0;
}

Float Win32KeyMappedTilt::GetForward() const
{
	return 0;
}

Float Win32KeyMappedTilt::GetLeft() const
{
	return -static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetMouseWheel();
}

Float Win32KeyMappedTilt::GetRight() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetMouseWheel();
}

}