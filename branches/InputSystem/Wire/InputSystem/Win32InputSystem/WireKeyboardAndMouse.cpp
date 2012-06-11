#include "WireKeyboardAndMouse.h"
#include "WireSystem.h"
#include <algorithm>

using namespace std;

namespace Wire
{

KeyboardAndMouse::KeyboardAndMouse(const PlatformKeyMapper* pPlatformKeyMapper) :
		mpPlatformKeyMapper(pPlatformKeyMapper), mpCurrentInputStateBuffer(NULL)
{
	mCapabilities.insert(BUTTONS);
	mCapabilities.insert(IR_AXIS);
	mCapabilities.insert(DIGITAL_AXIS);
}

KeyboardAndMouse::~KeyboardAndMouse()
{
	mpCurrentInputStateBuffer = NULL;
}

void KeyboardAndMouse::Capture()
{
	// TODO: Refactory.
	// Do nothing.
}

const char* KeyboardAndMouse::GetName() const
{
	return "[Keyboard, Mouse]";
}

Bool KeyboardAndMouse::GetButton(Button button) const
{
	if (mpCurrentInputStateBuffer == NULL)
		return false;

	if (button == BUTTON_A) 
	{
		return mpCurrentInputStateBuffer->GetRightMouseButtonDown();
	}
	else if (button == BUTTON_B) 
	{
		return mpCurrentInputStateBuffer->GetLeftMouseButtonDown();
	}
	else 
	{
		UInt key = mpPlatformKeyMapper->GetPlatformKeyForButton(button);
		return mpCurrentInputStateBuffer->GetKeyDown(key);
	}
}

Float KeyboardAndMouse::GetIRAxis(IRAxis axis) const
{
	if (mpCurrentInputStateBuffer == NULL)
		return 0;

	switch (axis)
	{
	case ANALOG_X:
		return (Float)mpCurrentInputStateBuffer->GetMouseX();
	case ANALOG_Y:
		return (Float)mpCurrentInputStateBuffer->GetMouseY();
	default:
		System::Assert("Unknown IR axis.", "WireKeyboardAndMouse.cpp", 87);
		return 0;
	}
}

Float KeyboardAndMouse::GetDigitalAxis(DigitalAxis axis) const
{
	if (mpCurrentInputStateBuffer == NULL)
		return 0;

	UInt key = mpPlatformKeyMapper->GetPlatformKeyForDigitalAxis(axis);
	if (mpCurrentInputStateBuffer->GetKeyDown(key))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

Float KeyboardAndMouse::GetAnalogAxis(AnalogAxis axis) const
{
	return 0;
}

void KeyboardAndMouse::SetCurrentInputStateBuffer(const InputStateBuffer* pCurrentKeyStateBuffer)
{
	mpCurrentInputStateBuffer = pCurrentKeyStateBuffer;
}

Bool KeyboardAndMouse::HasCapability(InputCapability capability) const
{
	set<InputCapability>::const_iterator iterator = find(mCapabilities.begin(), mCapabilities.end(), capability);
	return iterator != mCapabilities.end();
}

const set<InputCapability>& KeyboardAndMouse::GetCapabilities() const
{
	return mCapabilities;
}

}
