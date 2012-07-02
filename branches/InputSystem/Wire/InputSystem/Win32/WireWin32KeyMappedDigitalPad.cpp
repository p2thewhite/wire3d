#include "WireWin32KeyMappedDigitalPad.h"
#include "WireWin32InputDataBuffer.h"
#include "WireButton.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedDigitalPad, DigitalPad);

Win32KeyMappedDigitalPad::Win32KeyMappedDigitalPad(const InputDevice* pParent)
	: DigitalPad(pParent)
{
}

Win32KeyMappedDigitalPad::~Win32KeyMappedDigitalPad()
{
}

Bool Win32KeyMappedDigitalPad::GetDown() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_DOWN);
}

Bool Win32KeyMappedDigitalPad::GetLeft() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_LEFT);
}

Bool Win32KeyMappedDigitalPad::GetRight() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_RIGHT);
}

Bool Win32KeyMappedDigitalPad::GetUp() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_UP);
}

}