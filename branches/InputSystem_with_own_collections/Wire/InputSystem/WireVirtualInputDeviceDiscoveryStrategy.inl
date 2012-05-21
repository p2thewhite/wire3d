inline const TArray<VirtualInputDevice*>& VirtualInputDeviceDiscoveryStrategy::GetFoundVirtualInputDevices() const
{
	return mVirtualInputDevices;
}

inline void VirtualInputDeviceDiscoveryStrategy::AddInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener)
{
	mInputDeviceDiscoveryListeners.Append(pInputDeviceDiscoveryListener);
}

inline void VirtualInputDeviceDiscoveryStrategy::RemoveInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener)
{
	mInputDeviceDiscoveryListeners.Remove(pInputDeviceDiscoveryListener);
}

inline void VirtualInputDeviceDiscoveryStrategy::UpdateListeners() const
{
	for (UInt i = 0; i < mInputDeviceDiscoveryListeners.GetQuantity(); i++)
	{
		mInputDeviceDiscoveryListeners[i]->Update();
	}
}
