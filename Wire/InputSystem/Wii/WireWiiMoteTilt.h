// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIMOTETILT_H
#define WIREWIIMOTETILT_H

#include "WireTilt.h"

namespace Wire
{

/**
 * WiiMoteTilt enables the reading of a WiiMote tilting level.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and Tilt.
 **/
class WiiMoteTilt : public Tilt
{
	WIRE_DECLARE_RTTI;

public:
	WiiMoteTilt(const InputDevice* pParent);
	virtual ~WiiMoteTilt();

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
