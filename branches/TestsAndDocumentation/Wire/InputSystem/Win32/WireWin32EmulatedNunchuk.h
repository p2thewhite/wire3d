// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREWIN32EMULATEDNUNCHUK_H
#define WIREWIN32EMULATEDNUNCHUK_H

#include "WireNunchuk.h"

namespace Wire
{

/**
 * Win32EmulatedNunchuk is a Nunchuk emulation on keyboard.<br>
 * It runs on Windows platform and receives its data from Win32 API (therefore the Win32 prefix).
 *
 * See also: Win32InputSystem#DiscoverDevices() and Nunchuk.
 **/
class Win32EmulatedNunchuk : public Nunchuk
{
	WIRE_DECLARE_RTTI;

public:
	Win32EmulatedNunchuk(MainInputDevice* pMainInputDevice);
	virtual ~Win32EmulatedNunchuk();

	/// Registers all input capabilities an emulated Nunchuk has.
	virtual void SetUp();

};

}

#endif
