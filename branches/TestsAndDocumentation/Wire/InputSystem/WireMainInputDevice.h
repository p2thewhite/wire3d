// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREMAININPUTDEVICE_H
#define WIREMAININPUTDEVICE_H

#include "WireInputDevice.h"
#include "WireTArray.h"
#include "WireTHashTable.h"

namespace Wire
{

class InputDeviceExtension;

/**
 * MainInputDevice is an InputDevice that doesn't need any other input device to function properly (as opposed to an InputDeviceExtension).
 *
 * See also: InputSystem#GetMainDevice(UInt) const, InputSystem#GetMainDevicesCount(), Win32EmulatedWiiMote and WiiMote
 **/
class MainInputDevice : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	MainInputDevice();
	virtual ~MainInputDevice();

	/// Adds an extension to this main input device.
	void AddExtension(InputDeviceExtension* pExtension);
	/// Returns an input capability of this main input device that matches the passed type. Can also do a lookup for the input capabilities of his extensions.
	const InputCapability* GetCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const;
	/// Returns an extension of this main input device that matches the passed type.
	const InputDeviceExtension* GetExtension(const Rtti& rExtensionType) const;
	/// Returns an array containing all extensions of this main input device.
	const TArray<Pointer<InputDeviceExtension> >& GetExtensions() const;
	/// Returns true if this main input device has an extension that matches the passed type.
	Bool HasExtension(const Rtti& rExtensionType) const;
	/// Removes all extensions of this main input device.
	void RemoveAllExtensions();
	/// Returns true if this main input device has an input capability that matches the passed type. Can also do a lookup for the input capabilities of his extensions.
	Bool HasCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const;
	/// Sets the input data buffer of this main input device and also the input data buffers of his extensions.
	virtual void SetInputDataBuffer(const InputDataBuffer* pInputDataBuffer);

private:
	/// An array containing all extensions of this main input device.
	TArray<Pointer<InputDeviceExtension> > mExtensions;

};

typedef Pointer<MainInputDevice> MainInputDevicePtr;

}

#endif
