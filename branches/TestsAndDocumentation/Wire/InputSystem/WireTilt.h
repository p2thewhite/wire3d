// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRETILT_H
#define WIRETILT_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * Tilt enables the reading of an input device tilting level.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedAnalogPad and NunchukAnalogPad.
 **/
class Tilt : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Tilt(const InputDevice* pParent);
	virtual ~Tilt();

	/// Returns the tilting level in the Z-axis, normalized to (-1, 1).
	virtual Float GetBackward() const = 0;
	/// Returns the tilting level in the Z-axis, normalized to (-1, 1).
	virtual Float GetForward() const = 0;
	/// Returns the tilting level in the X-axis, normalized to (-1, 1).
	virtual Float GetLeft() const = 0;
	/// Returns the tilting level in the X-axis, normalized to (-1, 1).
	virtual Float GetRight() const = 0;

};

}

#endif
