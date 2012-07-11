// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireInputDevice.h"
#include "WireInputCapability.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, InputDevice, Object);

InputDevice::InputDevice()
{
}

InputDevice::~InputDevice()
{
}

const std::vector<const InputCapability*>& InputDevice::GetCapabilities() const
{
	return mReadOnlyCapabilities;
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
	mReadOnlyCapabilities.push_back(pInputCapability);
}
