// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIRENUNCHUK_H
#define WIRENUNCHUK_H

#include "WireInputDeviceExtension.h"

namespace Wire
{

/**
 * Nunchuk is a <a href="http://en.wikipedia.org/wiki/Marker_interface_pattern">marker interface</a>.<br>
 * Its RTTI type should be used as an argument for the extensions lookup method: MainInputDevice#GetExtension(const Rtti&) const.
 *
 * See also: Win32EmulatedNunchuk and WiiNunchuk
 **/
class Nunchuk : public InputDeviceExtension
{
	WIRE_DECLARE_RTTI;

public:
	Nunchuk(MainInputDevice* pMainInputDevice);
	virtual ~Nunchuk();

	/// Registers all input capabilities this input device should have.
	virtual void SetUp() = 0;

};

}

#endif
