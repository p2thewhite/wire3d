// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIMOTESWING_H
#define WIREWIIMOTESWING_H

#include "WireSwing.h"

namespace Wire
{

/**
 * WiiMoteSwing enables the reading of a WiiMote oscillatory movement (currently not implemented).
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Swing.
 **/
class WiiMoteSwing : public Swing
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteSwing(const InputDevice* pParent);
	virtual ~WiiMoteSwing();

	/// Returns the oscillation intensity in the Z-axis, normalized to (-1, 1).
	virtual Float GetBackward() const;
	/// Returns the oscillation intensity in the Y-axis, normalized to (-1, 1).
	virtual Float GetDown() const;
	/// Returns the oscillation intensity in the Z-axis, normalized to (-1, 1).
	virtual Float GetForward() const;
	/// Returns the oscillation intensity in the X-axis, normalized to (-1, 1).
	virtual Float GetLeft() const;
	/// Returns the oscillation intensity in the X-axis, normalized to (-1, 1).
	virtual Float GetRight() const;
	/// Returns the oscillation intensity in the Y-axis, normalized to (-1, 1).
	virtual Float GetUp() const;

};

}

#endif
