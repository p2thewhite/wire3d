inline UInt InputSystem::GetInputDevicesCount() const
{
	return mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices().GetQuantity();
}

inline const VirtualInputDevice* InputSystem::GetInputDevice(UInt index) const
{
	const TArray<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();

	if (index >= virtualInputDevices.GetQuantity())
	{
		return mpFallbackVirtualInputDevice;
	}
	else 
	{
		return virtualInputDevices[index];
	}
}

inline const PlatformKeyMapper* InputSystem::GetPlatformKeyMapper() const
{
	return mpPlatformKeyMapper;
}

inline VirtualInputDeviceDiscoveryStrategy* InputSystem::GetDevicesDiscoveryStrategy()
{
	return mpDevicesDiscoveryStrategy;
}
