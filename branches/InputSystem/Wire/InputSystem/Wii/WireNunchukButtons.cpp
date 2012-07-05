#include "WireNunchukButtons.h"
#include "WireWiiInputDataBuffer.h"
#include <wiiuse/wpad.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(NunchukButtons, Buttons);

NunchukButtons::NunchukButtons(const InputDevice* pParent)
	: Buttons(pParent)
{
}

NunchukButtons::~NunchukButtons()
{
}

Bool NunchukButtons::GetButton(Button button) const
{
	if (GetParent()->GetDataBuffer() == NULL)
	{
		return false;
	}

	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return false;
	}

	switch (button)
	{
	case BUTTON_A:
	case BUTTON_B:
	case BUTTON_1:
	case BUTTON_2:
	case BUTTON_MINUS:
	case BUTTON_PLUS:
	case BUTTON_HOME:
		return false;

	case BUTTON_Z:
		return (pData->btns_h & WPAD_NUNCHUK_BUTTON_Z) != 0;

	case BUTTON_C:
		return (pData->btns_h & WPAD_NUNCHUK_BUTTON_C) != 0;

	default:
		System::Assert("Unknown button.", "WireNunchukButtons.cpp", 46);
		return false;
	}
}

}