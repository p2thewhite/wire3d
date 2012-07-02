#include "WireWin32KeyMappedButtons.h"
#include "WireWin32InputDataBuffer.h"
#include "WireButton.h"
#include <Windows.h>
#include <WinUser.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(Win32KeyMappedButtons, Buttons);

Win32KeyMappedButtons::Win32KeyMappedButtons(const InputDevice* pParent)
	: Buttons(pParent)
{
}

Win32KeyMappedButtons::~Win32KeyMappedButtons()
{
}

Bool Win32KeyMappedButtons::GetButton(Button button) const
{
	switch (button)
	{
	case BUTTON_A:
		// letter a
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_LBUTTON);
	case BUTTON_B:
		// letter b
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_RBUTTON);
	case BUTTON_C:
		// letter c
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(0x43);
	case BUTTON_HOME:
		// escape
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_ESCAPE);
	case BUTTON_MINUS:
		// minus sign
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_SUBTRACT);
	case BUTTON_PLUS:
		// plus sign
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(VK_ADD);
	case BUTTON_Z:
		// letter z
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(0x5A);
	case BUTTON_1:
		// number 1
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(0x31);
	case BUTTON_2:
		// number 2
		return static_cast<const Win32InputDataBuffer*>(GetParent()->GetDataBuffer())->GetKey(0x32);
	}

	return false;
}

}