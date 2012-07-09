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
	Bool hasCapability = InputDevice::HasCapability(pCapabilityType);

	if (lookupExtensions && !hasCapability)
	{
		for (UInt i = 0; i < mExtensions.size(); i++)
		{
			if (mExtensions[i]->HasCapability(pCapabilityType))
			{
				return true;
			}
		}
	} 

	return hasCapability;
}

const InputCapability* MainInputDevice::GetCapability(const Rtti& pCapabilityType, Bool lookupExtensions) const
{
	const InputCapability* pInputCapability = InputDevice::GetCapability(pCapabilityType);

	if (lookupExtensions && pInputCapability == NULL)
	{
		for (UInt i = 0; i < mExtensions.size(); i++)
		{
			pInputCapability = mExtensions[i]->GetCapability(pCapabilityType);

			if (pInputCapability != NULL)
			{
				return pInputCapability;
			}
		}
	}

	return pInputCapability;
}

}