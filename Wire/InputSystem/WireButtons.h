// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREBUTTONS_H
#define WIREBUTTONS_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * Buttons enables the reading of an input device button state.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedButtons, NunchukButtons, WiiMoteButtons.
 **/
class Buttons : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:

	/**
	 * Button enumerates all platform buttons.
	 **/
	enum Button
	{
		BUTTON_A,
		BUTTON_B,
		BUTTON_C,
		BUTTON_Z,
		BUTTON_1,
		BUTTON_2,
		BUTTON_HOME,
		BUTTON_PLUS,
		BUTTON_MINUS
	};

	Buttons(const InputDevice* pParent);
	virtual ~Buttons();

	/// Returns true if a platform button is pressed and false otherwise.
	virtual Bool GetButton(Button button) const = 0;

};

}

#endif
