// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32KEYMAPPEDDIGITALPAD_H
#define WIREWIN32KEYMAPPEDDIGITALPAD_H

#include "WireDigitalPad.h"

namespace Wire
{

/**
 * Win32KeyMappedDigitalPad enables the reading of an emulated WiiMote digital pad direction.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const and DigitalPad.
 **/
class Win32KeyMappedDigitalPad : public DigitalPad
{
	WIRE_DECLARE_RTTI;

public:
	Win32KeyMappedDigitalPad(const InputDevice* pParent);
	virtual ~Win32KeyMappedDigitalPad();

	/// Returns true is the down button is pressed and false otherwise.
	virtual Bool GetDown() const;
	/// Returns true is the left button is pressed and false otherwise.
	virtual Bool GetLeft() const;
	/// Returns true is the right button is pressed and false otherwise.
	virtual Bool GetRight() const;
	/// Returns true is the up button is pressed and false otherwise.
	virtual Bool GetUp() const;

};

}

#endif
