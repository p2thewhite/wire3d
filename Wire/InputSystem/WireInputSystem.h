// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINPUTSYSTEM_H
#define WIREINPUTSYSTEM_H

#include "WireAnalogPad.h"
#include "WireButtons.h"
#include "WireDigitalPad.h"
#include "WireInputDeviceExtension.h"
#include "WireIR.h"
#include "WireMainInputDevice.h"
#include "WireNunchuk.h"
#include "WireShake.h"
#include "WireSwing.h"
#include "WireTilt.h"

namespace Wire
{

/**
 * InputSystem is the main class of all input system. It centralizes all input information.<br>
 * Since it communicates with the application layer, its specializations may contain platform dependent code.<br>
 * <br>
 * It works in a two-phase process:<br>
 *		1) Tries to discover all available input devices (DiscoverDevices()).<br>
 *		2) Consolidates all input data at his disposal (Capture()).<br>
 *
 * See also: Application#GetInputSystem() const, Win32InputSystem and WiiInputSystem
 **/
class InputSystem
{

public:
	InputSystem();
	virtual ~InputSystem();

	/// Tries to discover all available input devices.
	virtual void Capture() = 0;
	/// Consolidates all input data at his disposal.
	virtual Bool DiscoverDevices() = 0;
	/// Returns an input device by index.
	const InputDevice* GetDevice(UInt index) const;
	/// Returns an input device extension by index.
	const InputDeviceExtension* GetDeviceExtension(UInt index) const;
	/// Returns the total number of device extensions currently available.
	UInt GetDeviceExtensionsCount() const;
	/// Returns the total number of devices currently available (main devices + device extensions).
	UInt GetDevicesCount() const;
	/// Returns a main input device by index.
	const MainInputDevice* GetMainDevice(UInt index) const;
	/// Returns the total number of main input devices available.
	UInt GetMainDevicesCount() const;

protected:
	/// An array containing all devices available.
	TArray<InputDevicePtr> mDevices;
	/// An array containing all main input devices available.
	TArray<MainInputDevicePtr> mMainDevices;
	/// An array containing all input devices extensions available.
	TArray<InputDeviceExtensionPtr> mDeviceExtensions;

	/// Adds an input device to the correct internal collection. This method adds a main input device and all his input device extensions (recursively).
	void AddDevice(InputDevice* pInputDevice);
	/// Remove an input device from the correct internal collection. This method removes a main input device and all his input device extensions (recursively).
	void RemoveDevice(InputDevice* pInputDevice);

};

}

#endif
