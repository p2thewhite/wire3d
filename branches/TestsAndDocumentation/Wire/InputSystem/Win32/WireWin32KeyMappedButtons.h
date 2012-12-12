// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32KEYMAPPEDBUTTONS_H
#define WIREWIN32KEYMAPPEDBUTTONS_H

#include "WireButtons.h"

namespace Wire
{

/**
 * Win32KeyMappedButtons enables the reading of an emulated Nunchuk or WiiMote button state.<br>
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Buttons.
 **/
class Win32KeyMappedButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedButtons(const InputDevice* pParent);
	virtual ~Win32KeyMappedButtons();

	/// Returns true if a platform button is pressed and false otherwise.
	virtual Bool GetButton(Button button) const;

};

}

#endif
