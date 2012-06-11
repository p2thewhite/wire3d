inline Bool BaseVirtualInputDevice::HasCapability(InputCapability capability) const
{
	//return mCapabilities.Contains(capability);
	set<InputCapability>::const_iterator iterator = find(mCapabilities.begin(), mCapabilities.end(), capability);
	return iterator != mCapabilities.end();
}

//inline const THashSet<InputCapability>& BaseVirtualInputDevice::GetCapabilities() const
inline const set<InputCapability>& BaseVirtualInputDevice::GetCapabilities() const
{
	return mCapabilities;
}

inline InputDevice* BaseVirtualInputDevice::GetInputDevice(UInt index)
{
	//if (index >= mInputDevices.GetQuantity())
	if (index >= mInputDevices.size())
	{
		return NULL;
	}
	else
	{
		return mInputDevices[index];
	}
}
