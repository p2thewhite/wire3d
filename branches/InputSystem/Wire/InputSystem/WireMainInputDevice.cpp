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

MainInputDevice::MainInputDevice() :
	mExtensionsByAlias(5),
	mExtensions(8, 8)
{
}

MainInputDevice::~MainInputDevice()
{
}

void MainInputDevice::SetDataBuffer(const InputDataBuffer* pInputData)
{
	InputDevice::SetDataBuffer(pInputData);

	for (UInt i = 0; i < mExtensions.GetQuantity(); i++)
	{
		mExtensions[i]->SetDataBuffer(pInputData);
	}
}

void MainInputDevice::AddExtension(const Char* pAlias, InputDeviceExtension* pExtension)
{
	mExtensionsByAlias.Insert(pAlias, pExtension);
	mExtensions.Append(pExtension);
}

const InputDeviceExtension* MainInputDevice::GetExtensionByAlias(const Char* pAlias) const
{
	InputDeviceExtensionPtr* pValue = mExtensionsByAlias.Find(pAlias);

	if (pValue == NULL) 
	{
		return NULL;
	}
	else 
	{
		return *pValue;
	}
}

Bool MainInputDevice::HasExtension(const Char* pAlias) const
{
	return GetExtensionByAlias(pAlias) != NULL;
}

const TArray<Pointer<InputDeviceExtension> >& MainInputDevice::GetExtensions() const
{
	return mExtensions;
}

void MainInputDevice::RemoveAllExtensions()
{
	mExtensionsByAlias.RemoveAll();
	mExtensions.RemoveAll();
}

UInt MainInputDevice::GetExtensionsCount() const
{
	return mExtensions.GetQuantity();
}

Bool MainInputDevice::HasCapability(const Rtti& rCapabilityType,
	Bool lookupExtensions) const
{
	return GetCapability(rCapabilityType, lookupExtensions) != NULL;
}

const InputCapability* MainInputDevice::GetCapability(const Rtti& rCapabilityType,
	Bool lookupExtensions) const
{
	const InputCapability* pInputCapability = InputDevice::GetCapability(rCapabilityType);

	if (lookupExtensions && !pInputCapability)
	{
		for (UInt i = 0; i < mExtensions.GetQuantity(); i++)
		{
			pInputCapability = mExtensions[i]->GetCapability(rCapabilityType);
			if (pInputCapability)
			{
				return pInputCapability;
			}
		}
	}

	return pInputCapability;
}
