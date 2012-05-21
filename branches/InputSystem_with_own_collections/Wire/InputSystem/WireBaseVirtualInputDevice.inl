inline Bool BaseVirtualInputDevice::HasCapability(InputCapability capability) const
{
	return mCapabilities.Contains(capability);
}

inline const THashSet<InputCapability>& BaseVirtualInputDevice::GetCapabilities() const
{
	return mCapabilities;
}

inline InputDevice* BaseVirtualInputDevice::GetInputDevice(UInt index)
{
	if (index >= mInputDevices.GetQuantity())
	{
		return NULL;
	}
	else
	{
		return mInputDevices[index];
	}
}
