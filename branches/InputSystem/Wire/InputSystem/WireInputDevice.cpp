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
	:
	mCapabilitiesByType(16)
{
}

InputDevice::~InputDevice()
{
}

const TArray<const InputCapability*>& InputDevice::GetCapabilities() const
{
	return mReadOnlyCapabilities;
}

Bool InputDevice::HasCapability(const Rtti& rCapabilityType) const
{
	return GetCapability(rCapabilityType) != NULL;
}

const InputDataBuffer* InputDevice::GetDataBuffer() const
{
	return mpDataBuffer;
}

void InputDevice::SetDataBuffer(const InputDataBuffer* pDataBuffer)
{
	mpDataBuffer = pDataBuffer;
}

const InputCapability* InputDevice::GetCapability(const Rtti& rCapabilityType) const
{
	THashTable<const Rtti*, InputCapability*>::Iterator it(&mCapabilitiesByType);
	const Rtti* pKey = NULL;
	for (InputCapability** pValue = it.GetFirst(&pKey); pValue; pValue = it.GetNext(&pKey))
	{
		if (pKey->IsDerived(rCapabilityType))
		{
			return *pValue;
		}
	}

	return NULL;
}

void InputDevice::RegisterCapability(InputCapability* pInputCapability)
{
	mCapabilitiesByType.Insert(&pInputCapability->GetType(), pInputCapability);
	mReadOnlyCapabilities.Append(pInputCapability);
}
