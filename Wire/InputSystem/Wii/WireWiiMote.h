// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIIMOTE_H
#define WIREWIIMOTE_H

#include "WireMainInputDevice.h"

namespace Wire
{

/**
 * WiiMote is the main input device of the Wii platform.<br>
 * Currently there are 4 controller channels to which a WiiMote can connect to.<br>
 * Whenever a WiiMote connects to a controller channel, the input system will acknowledge this connection by creating
 * a new WiiMote object.
 *
 * See also: WiiInputSystem#DiscoverDevices() and MainInputDevice.
 **/
class WiiMote : public MainInputDevice
{
	WIRE_DECLARE_RTTI;

public:
	WiiMote(Int channel);
	virtual ~WiiMote();

	/// Returns the channel this WiiMote is connected to.
	UInt GetChannel() const;
	/// Registers all input capabilities a WiiMote has. This method is invoked automatically after the device discovery phase and should never be called directly by the user.
	virtual void SetUp();

private:
	/// The channel this WiiMote is connected to.
	UInt mChannel;

};

}

#endif
