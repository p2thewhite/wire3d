// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIMOTEBUTTONS_H
#define WIREWIIMOTEBUTTONS_H

#include "WireButtons.h"

namespace Wire
{

/**
 * WiiMoteButtons enables the reading of a WiiMote button state.<br>
 * WiiMotes have buttons: 'A', 'B', '1', '2', '+', '-' and 'HOME'.<br>
 * In order to know how to read the digital pad directions, refer to WiiMoteDigitalPad.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Buttons.
 **/
class WiiMoteButtons : public Buttons
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteButtons(const InputDevice* pParent);
	virtual ~WiiMoteButtons();

	/// Returns true if a platform button is pressed and false otherwise.
	virtual Bool GetButton(Button button) const;

};

}

#endif
