#ifndef INPUTDEVICEDISCOVERYSTRATEGY_H_
#define INPUTDEVICEDISCOVERYSTRATEGY_H_

#include "WireInputDeviceDiscoveryListener.h"
#include "WireInputDevice.h"
#include "WireMainInputDevice.h"
#include "WireInputDeviceExtension.h"
#include <vector>

namespace Wire
{

class InputDeviceDiscoveryStrategy
{

public:
	InputDeviceDiscoveryStrategy();
	virtual ~InputDeviceDiscoveryStrategy();

	void AddListener(InputDeviceDiscoveryListener* pListener);
	virtual void Discover() = 0;
	const std::vector<InputDevice*>& GetInputDevices() const;
	const std::vector<InputDeviceExtension*>& GetInputDeviceExtensions() const;
	const std::vector<MainInputDevice*>& GetMainInputDevices() const;
	void RemoveListener(InputDeviceDiscoveryListener* pListener);

protected:
	void AddInputDevice(InputDevice* pInputDevice);
	void NotifyListeners();

private:
	std::vector<InputDeviceExtension*> mInputDeviceExtensions;
	std::vector<InputDevice*> mInputDevices;
	std::vector<MainInputDevice*> mMainInputDevices;
	std::vector<InputDeviceDiscoveryListener*> mListeners;

};


}

#endif // INPUTDEVICEDISCOVERYSTRATEGY_H_
