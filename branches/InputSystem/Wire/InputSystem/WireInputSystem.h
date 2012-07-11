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

#include "WireInputSystemListener.h"
#include "WireMainInputDevice.h"
#include "WireInputDeviceExtension.h"
#include "WireTypes.h"
#include <vector>

namespace Wire
{

class InputSystem
{

public:
	InputSystem();
	virtual ~InputSystem();

	virtual void Capture() = 0;
	void DiscoverDevices();
	const InputDeviceExtension* GetDeviceExtension(UInt index) const;
	UInt GetDeviceExtensionsCount() const;
	const MainInputDevice* GetMainDevice(UInt index) const;
	UInt GetMainDevicesCount() const;
	const InputDevice* GetDevice(UInt index) const;
	UInt GetDevicesCount() const;
	void AddListener(InputSystemListener* pListener);
	void RemoveListener(InputSystemListener* pListener);

protected:
	std::vector<InputDevice*> mDevices;
	std::vector<MainInputDevice*> mMainDevices;
	std::vector<InputDeviceExtension*> mDeviceExtensions;
	std::vector<InputSystemListener*> mListeners;

	virtual void BeforeDevicesDiscovery();
	virtual void DoDevicesDiscovery() = 0;
	virtual void AfterDevicesDiscovery();
	void AddDevice(InputDevice* pInputDevice);
	void RemoveDevice(InputDevice* pInputDevice);
	void NotifyDevicesChange();

};

}

#endif
