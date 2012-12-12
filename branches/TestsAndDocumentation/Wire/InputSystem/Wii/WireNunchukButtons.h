// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRENUNCHUKBUTTONS_H
#define WIRENUNCHUKBUTTONS_H

#include "WireButtons.h"

namespace Wire
{

/**
 * NunchukButtons enables the reading of a Nunchuk button state.<br>
 * Nunchuks have only the buttons 'C' and 'Z'.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Buttons.
 **/
class NunchukButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	NunchukButtons(const InputDevice* pParent);
	virtual ~NunchukButtons();

	/// Returns true if a platform button is pressed and false otherwise.
	virtual Bool GetButton(Button button) const;

};

}

#endif
