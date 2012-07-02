#include "WireWin32KeyMappedShake.h"
#include "WireWin32InputDataBuffer.h"
#include "WireButton.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedShake, Shake);

Win32KeyMappedShake::Win32KeyMappedShake(const InputDevice* pParent)
	: Shake(pParent)
{
}

Win32KeyMappedShake::~Win32KeyMappedShake()
{
}

Float Win32KeyMappedShake::GetX() const
{
	return 0;
}

Float Win32KeyMappedShake::GetY() const
{
	return 0;
}

Float Win32KeyMappedShake::GetZ() const
{
	return 0;
}

}