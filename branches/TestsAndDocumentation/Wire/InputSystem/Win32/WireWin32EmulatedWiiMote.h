// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32EMULATEDWIIMOTE_H
#define WIREWIN32EMULATEDWIIMOTE_H

#include "WireMainInputDevice.h"

namespace Wire
{

/**
 * Win32EmulatedWiiMote is a WiiMote emulation on keyboard and mouse.<br>
 * It runs on Windows platform and receives its data from Win32 API (therefore the Win32 prefix).
 *
 * See also: Win32InputSystem#DiscoverDevices() and MainInputDevice.
 **/
class Win32EmulatedWiiMote : public MainInputDevice
{
	WIRE_DECLARE_RTTI;

public:
	Win32EmulatedWiiMote();
	virtual ~Win32EmulatedWiiMote();

	/// Registers all input capabilities an emulated WiiMote has.
	virtual void SetUp();

};

}

#endif
