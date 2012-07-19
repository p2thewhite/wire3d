// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "WireMainInputDevice.h"
#include "WireInputDeviceExtension.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI(Wire, MainInputDevice, InputDevice);

MainInputDevice::MainInputDevice()
{
}

MainInputDevice::~MainInputDevice()
{
}

void MainInputDevice::AddExtension(InputDeviceExtension* pInputDeviceExtension)
{
	mExtensions.Append(pInputDeviceExtension);
}

const InputDeviceExtension* MainInputDevice::GetExtension(UInt index) const
{
	return mExtensions[index];
}

UInt MainInputDevice::GetExtensionsCount() const
{
	return mExtensions.GetQuantity();
}

Bool MainInputDevice::HasCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const
{
	Bool hasCapability = InputDevice::HasCapability(rCapabilityType);

	if (lookupExtensions && !hasCapability)
	{
		for (UInt i = 0; i < mExtensions.GetQuantity(); i++)
		{
			if (mExtensions[i]->HasCapability(rCapabilityType))
			{
				return true;
			}
		}
	} 

	return hasCapability;
}

const InputCapability* MainInputDevice::GetCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const
{
	const InputCapability* pInputCapability = InputDevice::GetCapability(rCapabilityType);

	if (lookupExtensions && pInputCapability == NULL)
	{
		for (UInt i = 0; i < mExtensions.GetQuantity(); i++)
		{
			pInputCapability = mExtensions[i]->GetCapability(rCapabilityType);

			if (pInputCapability != NULL)
			{
				return pInputCapability;
			}
		}
	}

	return pInputCapability;
}
