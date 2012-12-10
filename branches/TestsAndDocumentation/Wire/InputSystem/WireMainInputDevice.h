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

class MainInputDevice : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	MainInputDevice();
	virtual ~MainInputDevice();

	void AddExtension(InputDeviceExtension* pExtension);
	const InputCapability* GetCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const;
	const InputDeviceExtension* GetExtension(const Rtti& rExtensionType) const;
	const TArray<Pointer<InputDeviceExtension> >& GetExtensions() const;
	Bool HasExtension(const Rtti& rExtensionType) const;
	void RemoveAllExtensions();
	Bool HasCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const;
	virtual void SetInputDataBuffer(const InputDataBuffer* pInputDataBuffer);

private:
	TArray<Pointer<InputDeviceExtension> > mExtensions;

};

typedef Pointer<MainInputDevice> MainInputDevicePtr;

}

#endif
