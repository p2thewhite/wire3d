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
#include "WireMainInputDevice.h"
#include "WireRtti.h"

namespace Wire
{

class Nunchuk : public InputDeviceExtension
{
	WIRE_DECLARE_RTTI;

public:
	Nunchuk(const MainInputDevice* pMainInputDevice);
	virtual ~Nunchuk();

	virtual void SetUp();

};

}

#endif
