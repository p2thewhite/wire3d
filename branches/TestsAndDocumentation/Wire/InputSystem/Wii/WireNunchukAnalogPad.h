// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRENUNCHUKANALOGPAD_H
#define WIRENUNCHUKANALOGPAD_H

#include "WireAnalogPad.h"

namespace Wire
{

/**
 * NunchukAnalogPad enables the reading of a Nunchuk analog pad direction.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and AnalogPad.
 **/
class NunchukAnalogPad : public AnalogPad
{
	WIRE_DECLARE_RTTI;

public:
	NunchukAnalogPad(const InputDevice* pParent);
	virtual ~NunchukAnalogPad();

	/// Returns the intensity of the down button press, normalized to (0, 1).
	virtual Float GetDown() const;
	/// Returns the intensity of the left button press, normalized to (0, 1).
	virtual Float GetLeft() const;
	/// Returns the intensity of the right button press, normalized to (0, 1).
	virtual Float GetRight() const;
	/// Returns the intensity of the up button press, normalized to (0, 1).
	virtual Float GetUp() const;

private:
	// Returns an average of the X-axis value, self-adjusting through time.
	Float GetX() const;
	// Returns an average of the Y-axis value, self-adjusting through time.
	Float GetY() const;

};

}

#endif
