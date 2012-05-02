#ifndef WIREINPUTSYSTEM_H_
#define WIREINPUTSYSTEM_H_

#include "WireVirtualInputDeviceDiscoveryStrategy.h"
#include "WireVirtualInputDevice.h"
#include "WirePlatformKeyMapper.h"
#include "WireTypes.h"
#include <vector>

using namespace std;

namespace Wire
{

class InputSystem
{
public:
	virtual ~InputSystem();

	virtual void Capture();

	Bool DiscoverInputDevices();
	inline UInt GetInputDevicesCount() const;
	inline const VirtualInputDevice* GetInputDevice(UInt index) const;
	inline const PlatformKeyMapper* GetPlatformKeyMapper() const;
	inline VirtualInputDeviceDiscoveryStrategy* GetDevicesDiscoveryStrategy();
protected:
	VirtualInputDeviceDiscoveryStrategy* mpDevicesDiscoveryStrategy;
	PlatformKeyMapper* mpPlatformKeyMapper;
	VirtualInputDevice* mpFallbackVirtualInputDevice;

	InputSystem();

	virtual void BeforeInputDevicesDiscovery();
	virtual void AfterInputDevicesDiscovery();
};

#include "WireInputSystem.inl"

}

//----------------------------------------------------------------------------
#ifdef WIRE_WIN
#include "Dx9/WireWindowsInputSystem.h"
#else
#include "Wii/WireWiiInputSystem.h"
#endif

#endif
