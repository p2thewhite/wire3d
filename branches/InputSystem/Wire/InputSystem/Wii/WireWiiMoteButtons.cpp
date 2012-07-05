#include "WireWiiMoteButtons.h"
#include "WireWiiMote.h"
#include "WireWiiInputDataBuffer.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(WiiMoteButtons, Buttons);

WiiMoteButtons::WiiMoteButtons(const InputDevice* pParent)
	: Buttons(pParent)
{
}

WiiMoteButtons::~WiiMoteButtons()
{
}

Bool WiiMoteButtons::GetButton(Button button) const
{
	const WPADData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetDataBuffer())->GetData();

	if (pData == NULL)
	{
		return false;
	}

	switch (button)
	{
	case BUTTON_A:
		return (pData->btns_h & WPAD_BUTTON_A) != 0;

	case BUTTON_B:
		return (pData->btns_h & WPAD_BUTTON_B) != 0;

	case BUTTON_1:
		return (pData->btns_h & WPAD_BUTTON_1) != 0;

	case BUTTON_2:
		return (pData->btns_h & WPAD_BUTTON_2) != 0;

	case BUTTON_MINUS:
		return (pData->btns_h & WPAD_BUTTON_MINUS) != 0;

	case BUTTON_PLUS:
		return (pData->btns_h & WPAD_BUTTON_PLUS) != 0;

	case BUTTON_HOME:
		return (pData->btns_h & WPAD_BUTTON_HOME) != 0;

	case BUTTON_Z:
	case BUTTON_C:
		return false;

	default:
		System::Assert("Unknown button.", "WireWiiMoteButtons.cpp", 48);
		return false;
	}
}

}