// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIINUNCHUK_H
#define WIREWIINUNCHUK_H

#include "WireNunchuk.h"

namespace Wire
{

/**
 * Nunchuk is the most important input device extension of the Wii platform.<br>
 * It's always plugged to a WiiMote and cannot work without being attached to it.<br> 
 * They share the same controller channel (see WiiMote).<br>
 * Currently this is the only WiiMote expansion supported.<br>
 *
 * See also: WiiInputSystem#DiscoverDevices() and InputDeviceExtension.
 **/
class WiiNunchuk : public Nunchuk
{
	WIRE_DECLARE_RTTI;

public:
	WiiNunchuk(MainInputDevice* pMainInputDevice);
	virtual ~WiiNunchuk();

	/// Registers all input capabilities a Nunchuk has. This method is invoked automatically after the device discovery phase and should never be called directly by the user.
	virtual void SetUp();
};

}

#endif
