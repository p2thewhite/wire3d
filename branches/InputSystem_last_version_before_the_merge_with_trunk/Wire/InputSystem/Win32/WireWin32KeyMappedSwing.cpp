#include "WireWin32KeyMappedSwing.h"
#include "WireWin32InputDataBuffer.h"
#include "WireButton.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedSwing, Swing);

Win32KeyMappedSwing::Win32KeyMappedSwing(const InputDevice* pParent)
	: Swing(pParent)
{
}

Win32KeyMappedSwing::~Win32KeyMappedSwing()
{
}

Float Win32KeyMappedSwing::GetBackward() const
{
	return 0;
}

Float Win32KeyMappedSwing::GetDown() const
{
	return 0;
}

Float Win32KeyMappedSwing::GetForward() const
{
	return 0;
}

Float Win32KeyMappedSwing::GetLeft() const
{
	return 0;
}

Float Win32KeyMappedSwing::GetRight() const
{
	return 0;
}

Float Win32KeyMappedSwing::GetUp() const
{
	return 0;
}

}