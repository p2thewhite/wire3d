// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINPUTDEVICEEXTENSION_H
#define WIREINPUTDEVICEEXTENSION_H

#include "WireInputDevice.h"
#include "WireMainInputDevice.h"

namespace Wire
{

/**
 * InputDeviceExtension is an InputDevice that needs to be plugged-in to a MainInputDevice to function properly.
 *
 * See also: InputSystem#GetDeviceExtension(UInt) const, InputSystem#GetDeviceExtensionsCount(), Nunchuk
 **/
class InputDeviceExtension : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	InputDeviceExtension(MainInputDevice* pMainInputDevice);
	virtual ~InputDeviceExtension();

protected:
	/// Returns a pointer to the main input device this extension is plugged-in to.
	const MainInputDevice* GetMainInputDevice() const;

private:
	/// A pointer to the main input device this extension is plugged-in to.
	MainInputDevice* mpMainInputDevice;

};

typedef Pointer<InputDeviceExtension> InputDeviceExtensionPtr;

}

#endif
