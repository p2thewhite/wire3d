inline const vector<VirtualInputDevice*>& VirtualInputDeviceDiscoveryStrategy::GetFoundVirtualInputDevices() const
{
	return mVirtualInputDevices;
}

inline void VirtualInputDeviceDiscoveryStrategy::AddInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener)
{
	mInputDeviceDiscoveryListeners.push_back(pInputDeviceDiscoveryListener);
}

inline void VirtualInputDeviceDiscoveryStrategy::RemoveInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener)
{
	mInputDeviceDiscoveryListeners.remove(pInputDeviceDiscoveryListener);
}

inline void VirtualInputDeviceDiscoveryStrategy::UpdateListeners() const
{
	list<InputDeviceDiscoveryListener*>::const_iterator iterator = mInputDeviceDiscoveryListeners.begin();
	while (iterator != mInputDeviceDiscoveryListeners.end())
	{
		(*iterator)->Update();
		iterator++;
	}
}
