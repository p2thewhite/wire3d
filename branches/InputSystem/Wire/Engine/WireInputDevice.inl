//inline const THashSet<InputCapability>& InputDevice::GetCapabilities() const
inline const set<InputCapability>& InputDevice::GetCapabilities() const
{
	return mCapabilities;
}

inline Bool InputDevice::HasCapability(InputCapability capability) const
{
	//return mCapabilities.Contains(capability);
	set<InputCapability>::const_iterator iterator = find(mCapabilities.begin(), mCapabilities.end(), capability);
	return iterator != mCapabilities.end();
}
