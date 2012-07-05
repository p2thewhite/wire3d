#include "WireMainInputDevice.h"
#include "WireInputDeviceExtension.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(MainInputDevice, InputDevice);

MainInputDevice::MainInputDevice()
{
}

MainInputDevice::~MainInputDevice()
{
}

void MainInputDevice::AddExtension(InputDeviceExtension* pInputDeviceExtension)
{
	mExtensions.push_back(pInputDeviceExtension);
}

const InputDeviceExtension* MainInputDevice::GetExtension(UInt index) const
{
	return mExtensions[index];
}

UInt MainInputDevice::GetExtensionsCount() const
{
	return mExtensions.size();
}

Bool MainInputDevice::HasCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const
{
	if (!lookupExtensions)
	{
		return InputDevice::HasCapability(pCapabilityType);
	}

	for (UInt i = 0; i < mExtensions.size(); i++)
	{
		if (mExtensions[i]->HasCapability(pCapabilityType))
		{
			return true;
		}
	}

	return false;
}

const InputCapability* MainInputDevice::GetCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const
{
	if (!lookupExtensions)
	{
		return InputDevice::GetCapability(pCapabilityType);
	}

	for (UInt i = 0; i < mExtensions.size(); i++)
	{
		const InputCapability* pInputCapability = mExtensions[i]->GetCapability(pCapabilityType);

		if (pInputCapability != NULL)
		{
			return pInputCapability;
		}
	}

	return NULL;
}

}