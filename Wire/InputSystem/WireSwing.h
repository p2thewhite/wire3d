// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESWING_H
#define WIRESWING_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * Swing enables the reading of an input device oscillatory movement.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedSwing, WiiMoteSwing and NunchukSwing.
 **/
class Swing : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Swing(const InputDevice* pParent);
	virtual ~Swing();

	/// Returns the oscillation intensity in the Z-axis, normalized to (-1, 1).
	virtual Float GetBackward() const = 0;
	/// Returns the oscillation intensity in the Y-axis, normalized to (-1, 1).
	virtual Float GetDown() const = 0;
	/// Returns the oscillation intensity in the Z-axis, normalized to (-1, 1).
	virtual Float GetForward() const = 0;
	/// Returns the oscillation intensity in the X-axis, normalized to (-1, 1).
	virtual Float GetLeft() const = 0;
	/// Returns the oscillation intensity in the X-axis, normalized to (-1, 1).
	virtual Float GetRight() const = 0;
	/// Returns the oscillation intensity in the Y-axis, normalized to (-1, 1).
	virtual Float GetUp() const = 0;

};

}

#endif
