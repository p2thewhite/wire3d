inline UInt InputSystem::GetInputDevicesCount() const
{
	//return mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices().GetQuantity();
	return mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices().size();
}

inline const VirtualInputDevice* InputSystem::GetInputDevice(UInt index) const
{
	//const TArray<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();
	const vector<VirtualInputDevice*>& virtualInputDevices = mpDevicesDiscoveryStrategy->GetFoundVirtualInputDevices();

	//if (index >= virtualInputDevices.GetQuantity())
	if (index >= virtualInputDevices.size())
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
