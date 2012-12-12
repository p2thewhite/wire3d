// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREANALOGPAD_H
#define WIREANALOGPAD_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * AnalogPad enables the reading of an input device analog pad direction.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedAnalogPad and NunchukAnalogPad.
 **/
class AnalogPad : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	AnalogPad(const InputDevice* pParent);
	virtual	~AnalogPad();

	/// Returns the intensity of the down button press, normalized to (0, 1).
	virtual Float GetDown() const = 0;
	/// Returns the intensity of the left button press, normalized to (0, 1).
	virtual Float GetLeft() const = 0;
	/// Returns the intensity of the right button press, normalized to (0, 1).
	virtual Float GetRight() const = 0;
	/// Returns the intensity of the up button press, normalized to (0, 1).
	virtual Float GetUp() const = 0;

};

}

#endif
