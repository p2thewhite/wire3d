// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINPUTDEVICE_H
#define WIREINPUTDEVICE_H

#include "WireInputDataBuffer.h"
#include "WireObject.h"
#include "WireTArray.h"

namespace Wire
{

class InputCapability;

/**
 * InputDevice is an upper level abstraction of a hardware device that produces input information.
 *
 * See also: InputSystem#GetDevice(UInt) const, InputSystem#GetDevicesCount(), MainInputDevice and InputDeviceExtension
 **/
class InputDevice : public Object
{
	WIRE_DECLARE_RTTI;

public:
	InputDevice();
	virtual ~InputDevice();

	/// Returns an array containing all the capabilities registered for this input device.
	const TArray<Pointer<InputCapability> >& GetCapabilities() const;
	/// Returns an input capability that matches the passed type.
	const InputCapability* GetCapability(const Rtti& rCapabilityType) const;
	/// Returns the current input data buffer for this input device.
	const InputDataBuffer* GetInputDataBuffer() const;
	/// Sets the current input data buffer for this input device.
	virtual void SetInputDataBuffer(const InputDataBuffer* pInputDataBuffer);
	/// Returns true if input device has capability that matches the passed type.
	Bool HasCapability(const Rtti& rCapabilityType) const;
	/// Registers all input capabilities this input device should have.
	virtual void SetUp() = 0;

protected:
	/// All the capabilities registered for this input device.
	TArray<Pointer<InputCapability> > mCapabilities;

	/// Registers an input capability.
	void RegisterCapability(InputCapability* pInputCapability);

private:
	// Current input data buffer.
	const InputDataBuffer* mpInputDataBuffer;

};

typedef Pointer<InputDevice> InputDevicePtr;

}

#endif
