#include "WireWin32KeyMappedIR.h"
#include "WireWin32InputDataBuffer.h"
#include "WireButton.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedIR, IR);

Win32KeyMappedIR::Win32KeyMappedIR(const InputDevice* pParent)
	: IR(pParent)
{
}

Win32KeyMappedIR::~Win32KeyMappedIR()
{
}

Float Win32KeyMappedIR::GetBackward() const
{
	return 0;
}

Float Win32KeyMappedIR::GetDown() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetMouseY();
}

Float Win32KeyMappedIR::GetForward() const
{
	return 0;
}

Float Win32KeyMappedIR::GetLeft() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetMouseX();
}

Float Win32KeyMappedIR::GetRight() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetMouseX();
}

Float Win32KeyMappedIR::GetUp() const
{
	return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetMouseY();
}

}