#pragma once
#ifndef WIREINPUTSYSTEMSTUB_H
#define WIREINPUTSYSTEMSTUB_H

#include "WireInputSystem.h"
#include "WireTArray.h"

namespace Wire
{

class InputSystemStub : public InputSystem
{
public:
	InputSystemStub();
	InputSystemStub(const TArray<MainInputDevicePtr>& rInitialInputDevices);
	virtual ~InputSystemStub();

	virtual void Capture();
	virtual Bool DiscoverDevices();

	inline void AddDevice_TEST(InputDevice* pInputDevice)
	{
		AddDevice(pInputDevice);
	}

	inline void RemoveDevice_TEST(InputDevice* pInputDevice)
	{
		RemoveDevice(pInputDevice);
	}

private:
	TArray<MainInputDevicePtr> mInitialInputDevices;

};

}

#endif