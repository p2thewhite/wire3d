// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNunchukButtons.h"
#include "WireWiiInputDataBuffer.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, NunchukButtons, Buttons);

NunchukButtons::NunchukButtons(const InputDevice* pParent)
	: Buttons(pParent)
{
}

NunchukButtons::~NunchukButtons()
{
}

Bool NunchukButtons::GetButton(Button button) const
{
	if (GetParent()->GetInputDataBuffer() == NULL)
	{
		return false;
	}

	const WPADWrapperData* pData = static_cast<const WiiInputDataBuffer*>(GetParent()->GetInputDataBuffer())->GetConstData();

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
		return (pData->btns_h & WPAD_WRAPPER_BUTTON_Z) != 0;

	case BUTTON_C:
		return (pData->btns_h & WPAD_WRAPPER_BUTTON_C) != 0;

	default:
		WIRE_ASSERT(false /* Unknown button */);
		return false;
	}
}
