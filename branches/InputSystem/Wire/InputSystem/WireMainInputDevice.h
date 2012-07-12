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
#include <vector>

namespace Wire
{

class InputDeviceExtension;

class MainInputDevice : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	MainInputDevice();
	virtual ~MainInputDevice();

	void AddExtension(InputDeviceExtension* pInputDevice);
	const InputDeviceExtension* GetExtension(UInt index) const;
	UInt GetExtensionsCount() const;
	Bool HasCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const;
	const InputCapability* GetCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const;

private:
	std::vector<const InputDeviceExtension*> mExtensions;

};

}

#endif
