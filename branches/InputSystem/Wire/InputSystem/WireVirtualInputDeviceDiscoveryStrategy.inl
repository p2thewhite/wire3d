//inline const TArray<VirtualInputDevice*>& VirtualInputDeviceDiscoveryStrategy::GetFoundVirtualInputDevices() const
inline const vector<VirtualInputDevice*>& VirtualInputDeviceDiscoveryStrategy::GetFoundVirtualInputDevices() const
{
	return mVirtualInputDevices;
}

inline void VirtualInputDeviceDiscoveryStrategy::AddInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener)
{
	//mInputDeviceDiscoveryListeners.Append(pInputDeviceDiscoveryListener);
	mInputDeviceDiscoveryListeners.push_back(pInputDeviceDiscoveryListener);
}

inline void VirtualInputDeviceDiscoveryStrategy::RemoveInputDeviceDiscoveryListener(InputDeviceDiscoveryListener* pInputDeviceDiscoveryListener)
{
	//mInputDeviceDiscoveryListeners.Remove(pInputDeviceDiscoveryListener);
	mInputDeviceDiscoveryListeners.remove(pInputDeviceDiscoveryListener);
}

inline void VirtualInputDeviceDiscoveryStrategy::UpdateListeners() const
{
	/*for (UInt i = 0; i < mInputDeviceDiscoveryListeners.GetQuantity(); i++)
	{
		mInputDeviceDiscoveryListeners[i]->Update();
	}*/
	list<InputDeviceDiscoveryListener*>::const_iterator iterator = mInputDeviceDiscoveryListeners.begin();
	while (iterator != mInputDeviceDiscoveryListeners.end())
	{
		(*iterator)->Update();
		iterator++;
	}
}
