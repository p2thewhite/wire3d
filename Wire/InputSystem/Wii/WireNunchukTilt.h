// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRENUNCHUKTILT_H
#define WIRENUNCHUKTILT_H

#include "WireTilt.h"

namespace Wire
{

/**
 * NunchukTilt enables the reading of a Nunchuk tilting level.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Tilt.
 **/
class NunchukTilt : public Tilt
{
	WIRE_DECLARE_RTTI;

public:
	NunchukTilt(const InputDevice* pParent);
	virtual ~NunchukTilt();

	/// Returns the tilting level in the Z-axis, normalized to (-1, 1).
	virtual Float GetBackward() const;
	/// Returns the tilting level in the Z-axis, normalized to (-1, 1).
	virtual Float GetForward() const;
	/// Returns the tilting level in the X-axis, normalized to (-1, 1).
	virtual Float GetLeft() const;
	/// Returns the tilting level in the X-axis, normalized to (-1, 1).
	virtual Float GetRight() const;

};

}

#endif
