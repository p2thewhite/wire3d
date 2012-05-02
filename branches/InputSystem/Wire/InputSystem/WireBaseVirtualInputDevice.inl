inline Bool BaseVirtualInputDevice::HasCapability(
		InputCapability capability) const
{
	set<InputCapability>::iterator iterator = find(mCapabilities.begin(),
			mCapabilities.end(), capability);
	return iterator != mCapabilities.end();
}

inline const set<InputCapability>& BaseVirtualInputDevice::GetCapabilities() const
{
	return mCapabilities;
}

inline InputDevice* BaseVirtualInputDevice::GetInputDevice(UInt index)
{
	if (index >= mInputDevices.size())
	{
		return NULL;
	}
	else
	{
		return mInputDevices[index];
	}
}
