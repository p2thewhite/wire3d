// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireNunchuk.h"
#include "WireNunchukAnalogPad.h"
#include "WireNunchukButtons.h"
#include "WireNunchukShake.h"
#include "WireNunchukSwing.h"
#include "WireNunchukTilt.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, Nunchuk, InputDeviceExtension);

Nunchuk::Nunchuk(MainInputDevice* pMainInputDevice)
	:
	InputDeviceExtension(pMainInputDevice)
{
}

Nunchuk::~Nunchuk()
{
}

void Nunchuk::SetUp()
{
	RegisterCapability(WIRE_NEW NunchukAnalogPad(this));
	RegisterCapability(WIRE_NEW NunchukButtons(this));
	RegisterCapability(WIRE_NEW NunchukShake(this));
	RegisterCapability(WIRE_NEW NunchukSwing(this));
	RegisterCapability(WIRE_NEW NunchukTilt(this));
}
