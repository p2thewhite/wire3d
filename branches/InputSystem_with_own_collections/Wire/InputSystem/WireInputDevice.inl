inline const THashSet<InputCapability>& InputDevice::GetCapabilities() const
{
	return mCapabilities;
}

inline Bool InputDevice::HasCapability(InputCapability capability) const
{
	return mCapabilities.Contains(capability);
}
