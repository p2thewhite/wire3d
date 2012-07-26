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
	mExtensionsByAlias(5)
{
}

MainInputDevice::~MainInputDevice()
{
}

void MainInputDevice::SetDataBuffer(const InputDataBuffer* pInputData)
{
	InputDevice::SetDataBuffer(pInputData);

	THashTable<const Char*, InputDeviceExtensionPtr >::Iterator it(&mExtensionsByAlias);
	const Char* pKey = NULL;
	for (InputDeviceExtensionPtr* pValue = it.GetFirst(&pKey); pValue; pValue = it.GetNext(&pKey))
	{
		(*pValue)->SetDataBuffer(pInputData);
	}
}

void MainInputDevice::AddExtension(const Char* pAlias, InputDeviceExtension* pExtension)
{
	mExtensionsByAlias.Insert(pAlias, pExtension);
	mReadOnlyExtensions.Append(pExtension);
}

void MainInputDevice::RemoveExtensionByAlias(const Char* pAlias)
{
	mExtensionsByAlias.Remove(pAlias);
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
	return mReadOnlyExtensions;
}

void MainInputDevice::RemoveAllExtensions()
{
	mExtensionsByAlias.RemoveAll();
	mReadOnlyExtensions.RemoveAll();
}

UInt MainInputDevice::GetExtensionsCount() const
{
	return mExtensionsByAlias.GetQuantity();
}

Bool MainInputDevice::HasCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const
{
	Bool hasCapability = InputDevice::HasCapability(rCapabilityType);

	if (lookupExtensions && !hasCapability)
	{
		THashTable<const Char*, InputDeviceExtensionPtr >::Iterator it(&mExtensionsByAlias);
		const Char* pKey = NULL;
		for (InputDeviceExtensionPtr* pValue = it.GetFirst(&pKey); pValue; pValue = it.GetNext(&pKey))
		{
			if ((*pValue)->HasCapability(rCapabilityType))
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
		THashTable<const Char*, InputDeviceExtensionPtr >::Iterator it(&mExtensionsByAlias);
		const Char* pKey = NULL;
		for (InputDeviceExtensionPtr* pValue = it.GetFirst(&pKey); pValue; pValue = it.GetNext(&pKey))
		{
			pInputCapability = (*pValue)->GetCapability(rCapabilityType);

			if (pInputCapability != NULL)
			{
				return pInputCapability;
			}
		}
	}

	return pInputCapability;
}
