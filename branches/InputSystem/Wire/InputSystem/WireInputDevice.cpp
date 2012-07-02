#include "WireInputDevice.h"
#include "WireInputCapability.h"

namespace Wire
{

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(InputDevice, Object);

InputDevice::InputDevice()
{
}

InputDevice::~InputDevice()
{
}

const std::vector<const InputCapability*>& InputDevice::GetCapabilities() const
{
	std::vector<const InputCapability*> inputCapabilities;
	std::map<const Rtti*, InputCapability*>::const_iterator i = mCapabilitiesByType.begin();

	while (i != mCapabilitiesByType.end())
	{
		inputCapabilities.push_back(i->second);
		i++;
	}

	return inputCapabilities;
}

Bool InputDevice::HasCapability(const Rtti& pCapabilityType) const
{
	return GetCapability(pCapabilityType) != NULL;
}

const InputDataBuffer* InputDevice::GetDataBuffer() const
{
	return mpDataBuffer;
}

void InputDevice::SetDataBuffer(const InputDataBuffer* pDataBuffer)
{
	mpDataBuffer = pDataBuffer;
}

const InputCapability* InputDevice::GetCapability(const Rtti& pCapabilityType) const
{
	std::map<const Rtti*, InputCapability*>::const_iterator i = mCapabilitiesByType.begin();

	while (i != mCapabilitiesByType.end())
	{
		if (i->first->IsDerived(pCapabilityType))
		{
			return i->second;
		}

		i++;
	}

	return NULL;
}

void InputDevice::RegisterCapability(InputCapability* pInputCapability)
{
	mCapabilitiesByType.insert(std::pair<const Rtti*, InputCapability*>(&pInputCapability->GetType(), pInputCapability));
}

}