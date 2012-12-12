// Wire3D by Pedro Boechat (pboechat@gmail.com)
// http://wire3d.googlecode.com
// Copyright(c) 2009-2012. All rights reserved.
// 
// The Wire3D source code is supplied under the terms of the LGPL and
// may not be copied or disclosed except in accordance with the terms of
// that agreement.

#pragma once
#ifndef WIREIR_H
#define WIREIR_H

#include "WireInputCapability.h"

namespace Wire
{

/**
 * IR enables the reading of an input device %IR tracking mechanism.
 *
 * See also: InputDevice#GetCapability(const Rtti&) const, Win32KeyMappedIR and WiiMoteIR.
 **/
class IR : public InputCapability
{
	WIRE_DECLARE_RTTI;

public:
	IR(const InputDevice* pParent);
	virtual ~IR();

	/// Returns the Z-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetBackward() const = 0;
	/// Returns the Y-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetDown() const = 0;
	/// Returns the Z-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetForward() const = 0;
	/// Returns the X-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetLeft() const = 0;
	/// Returns the X-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetRight() const = 0;
	/// Returns the Y-axis position of the IR tracker, normalized to (-1, 1).
	virtual Float GetUp() const = 0;

};

}

#endif
