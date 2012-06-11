#include "WireWiiMote.h"
#include "WireInputCapability.h"
#include "WireButton.h"
#include "WireAxis.h"
#include "WireSystem.h"
#include <wiiuse/wpad.h>

namespace Wire
{

WIRE_IMPLEMENT_RTTI(Wire, WiiMote, InputDevice);

WiiMote::WiiMote(const PlatformKeyMapper* pPlatformKeyMapper) :
		InputDevice(pPlatformKeyMapper)
{
	//mCapabilities.Insert(BUTTONS);
	//mCapabilities.Insert(DIGITAL_AXIS);
	//mCapabilities.Insert(IR_AXIS);
	mCapabilities.insert(BUTTONS);
	mCapabilities.insert(DIGITAL_AXIS);
	mCapabilities.insert(IR_AXIS);
}

WiiMote::~WiiMote()
{
	mpData = NULL;
}

void WiiMote::SetData(WPADData* pData)
{
	mpData = pData;
}

const char* WiiMote::GetName() const
{
	return "WiiMote";
}

Bool WiiMote::GetButton(Button button) const
{
	if (mpData == NULL) return false;

	UInt key;

	switch (button)
	{
	case BUTTON_A:
	case BUTTON_B:
	case BUTTON_1:
	case BUTTON_2:
	case BUTTON_MINUS:
	case BUTTON_PLUS:
	case BUTTON_HOME:
		key = mpPlatformKeyMapper->GetPlatformKeyForButton(button);
		return (mpData->btns_h & key) != 0;
	case BUTTON_Z:
	case BUTTON_C:
		return false;
	default:
		System::Assert("Unknown button.", "WireWiiMote.cpp", 58);
		return false;
	}
}

Float WiiMote::GetIRAxis(IRAxis axis) const
{
	if (mpData == NULL) return 0;

	if (!mpData->ir.valid)
	{
		return 0;
	}

	switch (axis)
	{
	case IR_X:
		return mpData->ir.x;
		break;
	case IR_Y:
		return mpData->ir.y;
		break;
	case IR_Z:
		return mpData->ir.z;
		break;
	default:
		System::Assert("Unknown IR axis.", "WireWiiMote.cpp", 83);
		return 0;
	}
}

Bool WiiMote::GetDigitalAxis(DigitalAxis axis) const
{
	if (mpData == NULL) return false;

	UInt key = mpPlatformKeyMapper->GetPlatformKeyForDigitalAxis(axis);
	return (mpData->btns_h & key) != 0;
}

Float WiiMote::GetAnalogAxis(AnalogAxis axis) const
{
	return 0;
}

}
