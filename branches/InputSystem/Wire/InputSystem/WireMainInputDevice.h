// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREMAININPUTDEVICE_H
#define WIREMAININPUTDEVICE_H

#include "WireInputDevice.h"
#include "WireTArray.h"
#include "WireTHashTable.h"

namespace Wire
{

class InputDeviceExtension;

class MainInputDevice : public InputDevice
{
	WIRE_DECLARE_RTTI;

public:
	MainInputDevice();
	virtual ~MainInputDevice();

	virtual void SetDataBuffer(const InputDataBuffer* pInputData);

	void AddExtension(const Char* pAlias, InputDeviceExtension* pInputDevice);
	const InputDeviceExtension* GetExtensionByAlias(const Char* pAlias) const;
	Bool HasExtension(const Char* pAlias) const;

	void AddExtension(InputDeviceExtension* pInputDevice);
	const TArray<Pointer<InputDeviceExtension> >& GetExtensions() const;
	void RemoveAllExtensions();
	UInt GetExtensionsCount() const;

	Bool HasCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const;
	const InputCapability* GetCapability(const Rtti& rCapabilityType, Bool lookupExtensions) const;

private:
	THashTable<const Char*, Pointer<InputDeviceExtension> > mExtensionsByAlias;
	TArray<Pointer<InputDeviceExtension> > mExtensions;
};

typedef Pointer<MainInputDevice> MainInputDevicePtr;

}

#endif
