#ifndef INPUTSYSTEM_H_
#define INPUTSYSTEM_H_

#include "WireInputDeviceDiscoveryStrategy.h"
#include "WireInputDeviceDiscoveryListener.h"
#include "WireMainInputDevice.h"
#include "WireInputDeviceExtension.h"
#include "WireTypes.h"

namespace Wire
{

class InputSystem
{

public:
	InputSystem();
	virtual ~InputSystem();

	virtual void Capture() = 0;
	void DiscoverInputDevices();
	const InputDeviceExtension* GetInputDeviceExtension(UInt index) const;
	UInt GetInputDeviceExtensionsCount() const;
	const MainInputDevice* GetMainInputDevice(UInt index) const;
	UInt GetMainInputDevicesCount() const;
	const InputDevice* GetInputDevice(UInt index) const;
	UInt GetInputDevicesCount() const;
	void AddInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pListener);
	void RemoveInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pListener);

protected:
	InputDeviceDiscoveryStrategy* mpInputDeviceDiscoveryStrategy;

	virtual void AfterInputDeviceDiscovery();
	virtual void BeforeInputDeviceDiscovery();

};

}

#endif // INPUTSYSTEM_H_
