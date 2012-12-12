// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIMOTESHAKE_H
#define WIREWIIMOTESHAKE_H

#include "WireShake.h"

namespace Wire
{

/**
 * WiiMoteShake enables the reading of a WiiMote jolting movement.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Shake.
 **/
class WiiMoteShake : public Shake
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteShake(const InputDevice* pParent);
	virtual ~WiiMoteShake();

	/// Returns the jolting intensity in the X-axis, normalized to (0, 1).
	virtual Float GetX() const;
	/// Returns the jolting intensity in the Y-axis, normalized to (0, 1).
	virtual Float GetY() const;
	/// Returns the jolting intensity in the Z-axis, normalized to (0, 1).
	virtual Float GetZ() const;

};

}

#endif
