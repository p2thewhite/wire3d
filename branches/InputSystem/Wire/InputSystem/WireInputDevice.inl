inline const set<InputCapability>& InputDevice::GetCapabilities() const
{
	return mCapabilities;
}

inline Bool InputDevice::HasCapability(InputCapability capability) const
{
	set<InputCapability>::iterator iterator = find(mCapabilities.begin(),
			mCapabilities.end(), capability);
	return iterator != mCapabilities.end();
}
