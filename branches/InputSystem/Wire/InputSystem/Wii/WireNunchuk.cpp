#include "WireNunchuk.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI(Wire, Nunchuk, InputDevice);

Nunchuk::Nunchuk(const PlatformKeyMapper* pPlatformKeyMapper) :
		WiiExtension(pPlatformKeyMapper)
{
	//mCapabilities.Insert(BUTTONS);
	//mCapabilities.Insert(ANALOG_AXIS);
	mCapabilities.insert(BUTTONS);
	mCapabilities.insert(ANALOG_AXIS);
}

Nunchuk::~Nunchuk()
{
	mpData = NULL;
}

void Nunchuk::SetData(WPADData* pData)
{
	mpData = pData;
}

const char* Nunchuk::GetName() const
{
	return "Nunchuk";
}

Bool Nunchuk::GetButton(Button button) const
{
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
		return false;
	case BUTTON_Z:
	case BUTTON_C:
		key = mpPlatformKeyMapper->GetPlatformKeyForButton(button);
		return (mpData->btns_h & key) != 0;
	default:
		System::Assert("Unknown button.", "WireNunchuk.cpp", 50);
		return false;
	}
}

Float Nunchuk::GetIRAxis(IRAxis axis) const
{
	return 0;
}

Bool Nunchuk::GetDigitalAxis(DigitalAxis axis) const
{
	return 0;
}

Float Nunchuk::GetAnalogAxis(AnalogAxis axis) const
{
	switch (axis)
	{
	case ANALOG_X:
		return mpData->exp.nunchuk.js.pos.x;
	case ANALOG_Y:
		return mpData->exp.nunchuk.js.pos.y;
	default:
		System::Assert("Unknown analog axis.", "WireNunchuk.cpp", 74);
		return 0;
	}
}

}
