// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREINPUTCAPABILITY_H
#define WIREINPUTCAPABILITY_H

#include "WireInputDevice.h"

namespace Wire
{

/**
 * InputCapability is an upper level abstraction of an input device capability (i.e: buttons, pads, motion).
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, AnalogPad, Buttons, DigitalPad, IR, Shake, Swing and Tilt.
 **/
class InputCapability : public Object
{
	WIRE_DECLARE_RTTI;

public:
	InputCapability(const InputDevice* pParent);
	virtual ~InputCapability();

protected:
	/// Returns a pointer to the input device that owns the capability.
	const InputDevice* GetParent() const;

private:
	// Pointer to the input device that owns the capability.
	const InputDevice* mpParent;

};

typedef Pointer<InputCapability> InputCapabilityPtr;

}

#endif
