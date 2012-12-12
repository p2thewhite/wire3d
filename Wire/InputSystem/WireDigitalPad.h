// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREDIGITALPAD_H
#define WIREDIGITALPAD_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * DigitalPad enables the reading of an input device digital pad direction.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedDigitalPad, WiiMoteDigitalPad.
 **/
class DigitalPad : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	DigitalPad(const InputDevice* pParent);
	virtual ~DigitalPad();

	/// Returns true is the down button is pressed and false otherwise.
	virtual Bool GetDown() const = 0;
	/// Returns true is the left button is pressed and false otherwise.
	virtual Bool GetLeft() const = 0;
	/// Returns true is the right button is pressed and false otherwise.
	virtual Bool GetRight() const = 0;
	/// Returns true is the up button is pressed and false otherwise.
	virtual Bool GetUp() const = 0;

};

}

#endif
