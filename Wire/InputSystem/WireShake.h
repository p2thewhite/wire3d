// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
//
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRESHAKE_H
#define WIRESHAKE_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * Shake enables the reading of an input device jolting movement.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedShake, WiiMoteShake and NunchukShake.
 **/
class Shake : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	Shake(const InputDevice* pParent);
	virtual ~Shake();

	/// Returns the jolting intensity in the X-axis, normalized to (0, 1).
	virtual Float GetX() const = 0;
	/// Returns the jolting intensity in the Y-axis, normalized to (0, 1).
	virtual Float GetY() const = 0;
	/// Returns the jolting intensity in the Z-axis, normalized to (0, 1).
	virtual Float GetZ() const = 0;

};

}

#endif
